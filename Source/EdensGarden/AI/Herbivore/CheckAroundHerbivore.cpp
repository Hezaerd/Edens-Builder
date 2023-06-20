// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckAroundHerbivore.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../../Basic/Planet.h"
#include "../../Characters/Herbivore.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Animation/AnimSequence.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#define HALF 0.5
void UCheckAroundHerbivore::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return;
	}
	ACharacter* Character = Controller->GetCharacter();
	UBlackboardComponent* Blackboard = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	AHerbivore* Animal = nullptr;

	if (!Blackboard)
	{
		return;
	}
	if (Character)
	{
		Animal = Cast<AHerbivore>(Character);
	}
	if (!Blackboard->GetValueAsBool("CanMate"))
	{
		if (Animal->otherHerbivores.Num() > 0)
		{
			AHerbivore* OtherHerbivore = nullptr;
			double ClosestDistance = 0;
			AAIController* OtherController = nullptr;
			for (int i = 0; i < Animal->otherHerbivores.Num(); ++i)
			{
				double Distance = FVector::Dist(Animal->otherHerbivores[i]->GetActorLocation(), Animal->GetActorLocation());
				if (Distance < ClosestDistance || ClosestDistance == 0 && !Animal->IsDead)
				{
					ClosestDistance = Distance;
					OtherHerbivore = Animal->otherHerbivores[i];
					OtherController = Cast<AAIController>(OtherHerbivore->GetController());
				}
			}
			if (!OtherHerbivore || !OtherController)
			{
				PlayOnce = true;
				return;
			}
			if (!Animal->IsDead && OtherHerbivore->otherHerbivore == nullptr && Animal->otherHerbivore == nullptr
				&& !OtherController->GetBlackboardComponent()->GetValueAsBool("CanMate"))
			{
				//Check stats here!

				if (Animal->ReproductionPoints > Animal->MinReproductionPoints && Animal->age >= Animal->AdultAge
					&& OtherHerbivore->ReproductionPoints > OtherHerbivore->MinReproductionPoints && OtherHerbivore->age >= OtherHerbivore->AdultAge
					&& OtherHerbivore->Health > 0)
				{
					APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
					USphereComponent* SphereComponent = Cast<USphereComponent>(Planet->GetComponentByClass(USphereComponent::StaticClass()));
					Blackboard->SetValueAsBool("CanMate", true);
					Blackboard->SetValueAsObject("OtherHerbivore", OtherHerbivore);
					FVector Difference = OtherHerbivore->GetActorLocation() - Animal->GetActorLocation();
					FVector Point = Animal->GetActorLocation() + (HALF * Difference);
					FVector PointPlanet = Point - Planet->GetActorLocation();
					PointPlanet.Normalize();

					FVector SurfacePos = Planet->GetActorLocation() + PointPlanet * SphereComponent->GetScaledSphereRadius();

					Blackboard->SetValueAsVector("MatingVector", SurfacePos);
					Animal->otherHerbivore = OtherHerbivore;
					OtherController->GetBlackboardComponent()->SetValueAsBool("CanMate", true);
					OtherController->GetBlackboardComponent()->SetValueAsObject("OtherHerbivore", Animal);
					OtherController->GetBlackboardComponent()->SetValueAsVector("MatingVector", SurfacePos);
					FVector VecDif = Animal->GetActorLocation() - Planet->GetActorLocation();
					FRotator Rot = FRotationMatrix::MakeFromZ(VecDif).Rotator();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Animal->VFX_Horny, Animal->GetActorLocation(), Rot, FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OtherHerbivore->VFX_Horny, OtherHerbivore->GetActorLocation(), Rot, FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);
					if (PlayOnce && Animal->Health > 0)
					{
						Animal->GetMesh()->PlayAnimation(Animal->Mating, true);

						// TODO: Play niagara effect for animal 1
						OtherHerbivore->GetMesh()->PlayAnimation(Animal->Mating, true);
						// TODO: Play niagara effect for animal 2

						PlayOnce = false;
					}
					OtherHerbivore->otherHerbivore = Animal;
				}
			}
			else if (Animal->otherHerbivore == OtherHerbivore)
			{
				Blackboard->SetValueAsBool("CanMate", true);
				Blackboard->SetValueAsObject("OtherHerbivore", OtherHerbivore);
				Blackboard->SetValueAsVector("MatingVector", OtherHerbivore->GetActorLocation());
			}
			else if (Animal->otherHerbivore != OtherHerbivore)
			{
				PlayOnce = true;
				Blackboard->SetValueAsBool("CanMate", false);
				Blackboard->SetValueAsObject("OtherHerbivore", nullptr);
			}
			else if (Animal->otherHerbivore != Blackboard->GetValueAsObject("OtherHerbivore"))
			{
				PlayOnce = true;
				Blackboard->SetValueAsBool("CanMate", false);
				OtherController->GetBlackboardComponent()->SetValueAsBool("CanMate", false);
				Blackboard->SetValueAsObject("OtherHerbivore", nullptr);
			}
		}
	}
	else if (Animal->otherHerbivore->otherHerbivore == nullptr || Animal->otherHerbivore->otherHerbivore != Animal || Animal->otherHerbivore->Chaser != nullptr)
	{
		Blackboard->SetValueAsBool("CanMate", false);
	}
}