// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckAroundCarnivore.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../../Basic/Planet.h"
#include "../../Characters/Carnivore.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Animation/AnimSequence.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
void UCheckAroundCarnivore::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return;
	}
	ACharacter* Character = Controller->GetCharacter();
	UBlackboardComponent* Blackboard = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	ACarnivore* Animal = nullptr;

	if (!Blackboard)
	{
		return;
	}
	if (Character)
	{
		Animal = Cast<ACarnivore>(Character);
	}
	if (!Blackboard->GetValueAsBool("CanMate") && !Blackboard->GetValueAsBool("CanEatCarnivore") && !Blackboard->GetValueAsBool("CanEat"))
	{
		if (Animal->otherCarnivores.Num() > 0)
		{
			ACarnivore* OtherCarnivore = nullptr;
			double ClosestDistance = 0;
			AAIController* OtherController = nullptr;
			for (int i = 0; i < Animal->otherCarnivores.Num(); ++i)
			{
				double Distance = FVector::Dist(Animal->otherCarnivores[i]->GetActorLocation(), Animal->GetActorLocation());
				if (Distance < ClosestDistance || ClosestDistance == 0 && !Animal->IsDead)
				{
					ClosestDistance = Distance;
					OtherCarnivore = Animal->otherCarnivores[i];
					OtherController = Cast<AAIController>(OtherCarnivore->GetController());
				}
			}
			if (!OtherCarnivore || !OtherController)
			{
				return;
			}
			if (!Animal->IsDead && OtherCarnivore->otherCarnivore == nullptr && Animal->otherCarnivore == nullptr
				&& !OtherController->GetBlackboardComponent()->GetValueAsBool("CanMate"))
			{
				//Check stats here!

				if (Animal->ReproductionPoints > Animal->MinReproductionPoints && Animal->age >= Animal->AdultAge
					&& OtherCarnivore->ReproductionPoints > OtherCarnivore->MinReproductionPoints && OtherCarnivore->age >= OtherCarnivore->AdultAge
					&& (Animal->Health > 0 && OtherCarnivore->Health > 0))
				{
					APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
					USphereComponent* SphereComponent = Cast<USphereComponent>(Planet->GetComponentByClass(USphereComponent::StaticClass()));
					Blackboard->SetValueAsBool("CanMate", true);
					Blackboard->SetValueAsObject("OtherCarnivore", OtherCarnivore);
					FVector Difference = OtherCarnivore->GetActorLocation() - Animal->GetActorLocation();
					FVector Point = Animal->GetActorLocation() + (0.5f * Difference);
					FVector PointPlanet = Point - Planet->GetActorLocation();
					PointPlanet.Normalize();

					FVector SurfacePos = Planet->GetActorLocation() + PointPlanet * SphereComponent->GetScaledSphereRadius();

					Blackboard->SetValueAsVector("MatingVector", SurfacePos);
					Animal->otherCarnivore = OtherCarnivore;
					OtherController->GetBlackboardComponent()->SetValueAsBool("CanMate", true);
					OtherController->GetBlackboardComponent()->SetValueAsObject("OtherCarnivore", Animal);
					OtherController->GetBlackboardComponent()->SetValueAsVector("MatingVector", SurfacePos);
					FVector VecDiff = Animal->GetActorLocation() - Planet->GetActorLocation();
					FRotator Rot = FRotationMatrix::MakeFromZ(VecDiff).Rotator();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Animal->VFX_Horny, Animal->GetActorLocation(), Rot, FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OtherCarnivore->VFX_Horny, OtherCarnivore->GetActorLocation(), Rot, FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);
					if (PlayOnce)
					{
						Animal->GetMesh()->PlayAnimation(Animal->Mating, true);
						OtherCarnivore->GetMesh()->PlayAnimation(Animal->Mating, true);
						PlayOnce = false;
					}
					OtherCarnivore->otherCarnivore = Animal;
				}
			}
			else if (Animal->otherCarnivore == OtherCarnivore)
			{
				Blackboard->SetValueAsBool("CanMate", true);
				Blackboard->SetValueAsObject("OtherCarnivore", OtherCarnivore);
				Blackboard->SetValueAsVector("MatingVector", OtherCarnivore->GetActorLocation());
			}
			else if (Animal->otherCarnivore != OtherCarnivore)
			{
				PlayOnce = true;
				Blackboard->SetValueAsBool("CanMate", false);
				Blackboard->SetValueAsObject("OtherCarnivore", nullptr);
			}
			else if (Animal->otherCarnivore != Blackboard->GetValueAsObject("OtherCarnivore"))
			{
				PlayOnce = true;
				Blackboard->SetValueAsBool("CanMate", false);
				OtherController->GetBlackboardComponent()->SetValueAsBool("CanMate", false);
				Blackboard->SetValueAsObject("OtherCarnivore", nullptr);
			}
		}
	}
	else if (Animal->otherCarnivore == nullptr)
	{
		PlayOnce = true;
		Blackboard->SetValueAsBool("CanMate", false);
		return;
	}
	else if (Animal->otherCarnivore->otherCarnivore == nullptr || Animal->otherCarnivore->otherCarnivore != Animal)
	{
		PlayOnce = true;
		Blackboard->SetValueAsBool("CanMate", false);
	}
}