// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckForFoodCarnivore.h"
#include "AIController.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/SphereComponent.h"
#include "../../Basic/Planet.h"
#include "../../Characters/Herbivore.h"
#include "../../Characters/Carnivore.h"
#include "../../Characters/BasicAnimal.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Animation/AnimSequence.h"
void UCheckForFoodCarnivore::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return;
	}
	ACharacter* Character = Controller->GetCharacter();
	UBlackboardComponent* Blackboard = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	Blackboard->SetValueAsBool("CanEat", false);
	if (Blackboard->GetValueAsBool("CanMate"))
		return;
	ACarnivore* Animal = nullptr;
	if (!Blackboard)
	{
		return;
	}
	if (Character)
	{
		Animal = Cast<ACarnivore>(Character);
	}

	ABasicAnimal* ClosestAnimal = nullptr;
	AHerbivore* ClosestHerbivore = nullptr;
	ACarnivore* ClosestCarnivore = nullptr;
	double ClosestDistance = 0;
	for (int i = 0; i < Animal->Herbivores.Num(); ++i)
	{
		double Distance = FVector::Dist(Animal->Herbivores[i]->GetActorLocation(), Animal->GetActorLocation());
		if ((Distance < ClosestDistance || ClosestDistance == 0 && !Animal->IsDead))
		{
			ClosestDistance = Distance;
			ClosestHerbivore = Animal->Herbivores[i];
		}
	}
	for (int i = 0; i < Animal->EatableCarnivores.Num(); ++i)
	{
		double Distance = FVector::Dist(Animal->EatableCarnivores[i]->GetActorLocation(), Animal->GetActorLocation());
		if ((Distance < ClosestDistance || ClosestDistance == 0 && !Animal->IsDead))
		{
			ClosestDistance = Distance;
			ClosestCarnivore = Animal->EatableCarnivores[i];
		}
	}
	Blackboard->SetValueAsBool("CanEatCarnivore", false);
	Blackboard->SetValueAsVector("EatVector", Animal->GetActorLocation());
	Blackboard->SetValueAsBool("CanEatCarnivore", false);
	if (Animal->Chaser != nullptr)
	{
		Animal->otherEatableCarnivore = Cast<ACarnivore>(Animal->Chaser);
		if (Animal->Chaser->Health < 0)
		{
			Animal->Chaser = nullptr;
			Blackboard->SetValueAsBool("EatPreyCarnivore", true);
			Blackboard->SetValueAsBool("CanEat", false);
			return;
		}

		Blackboard->SetValueAsBool("CanEatCarnivore", true);
		APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
		FVector DistanceVec = Animal->Chaser->GetActorLocation() - Planet->GetActorLocation();
		DistanceVec.Normalize();
		USphereComponent* PlanetSphere = Cast<USphereComponent>(Planet->GetComponentByClass(USphereComponent::StaticClass()));

		FVector surfacepos = Planet->GetActorLocation() + DistanceVec * PlanetSphere->GetScaledSphereRadius();

		Blackboard->SetValueAsVector("EatVector", surfacepos);

		Animal->Chaser->Chaser = Animal;
		return;
	}
	if (ClosestCarnivore)
	{
		if (Animal->CurrentHunger < 0.01 * Animal->HungerThreshold * Animal->MaxHunger)
		{
			if (ClosestCarnivore->Health < 0)
			{
				Blackboard->SetValueAsBool("EatPreyCarnivore", true);
				Blackboard->SetValueAsBool("CanEat", false);
			}
			Animal->otherEatableCarnivore = ClosestCarnivore;
			Blackboard->SetValueAsBool("CanEatCarnivore", true);
			APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
			FVector DistanceVec = ClosestCarnivore->GetActorLocation() - Planet->GetActorLocation();
			DistanceVec.Normalize();
			USphereComponent* PlanetSphere = Cast<USphereComponent>(Planet->GetComponentByClass(USphereComponent::StaticClass()));

			FVector surfacepos = Planet->GetActorLocation() + DistanceVec * PlanetSphere->GetScaledSphereRadius();

			Blackboard->SetValueAsVector("EatVector", surfacepos);

			ClosestCarnivore->Chaser = Animal;
			return;
		}
	}
	if (!ClosestHerbivore)
	{
		Blackboard->SetValueAsBool("EatPrey", false);
		Blackboard->SetValueAsVector("EatVector", Animal->GetActorLocation());
		Blackboard->SetValueAsBool("CanEat", false);
		return;
	}

	if (Animal->CurrentHunger < 0.01 * Animal->HungerThreshold * Animal->MaxHunger)
	{
		Blackboard->SetValueAsBool("CanEat", true);
		APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
		FVector DistanceVec = ClosestHerbivore->GetActorLocation() - Planet->GetActorLocation();
		DistanceVec.Normalize();
		USphereComponent* PlanetSphere = Cast<USphereComponent>(Planet->GetComponentByClass(USphereComponent::StaticClass()));

		FVector surfacepos = Planet->GetActorLocation() + DistanceVec * PlanetSphere->GetScaledSphereRadius();

		Blackboard->SetValueAsVector("EatVector", surfacepos);
		Animal->otherHerbivore = ClosestHerbivore;
		Animal->otherHerbivore->IsBeingChased = true;
		UBlackboardComponent* PreyBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(Animal->otherHerbivore);
		PreyBlackboard->SetValueAsBool("TmpRunAway", true);
		if (PlayOnce)
		{
			ClosestHerbivore->GetMesh()->PlayAnimation(ClosestHerbivore->Walking, true);
			PlayOnce = false;
		}
		
		Animal->otherHerbivore->Chaser = Animal;
		if (ClosestHerbivore->Health < 0)
		{
			Blackboard->SetValueAsBool("EatPrey", true);
			Blackboard->SetValueAsBool("CanEat", false);
		}
	}
	else
	{
		UBlackboardComponent* PreyBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(Animal->otherHerbivore);
		if (Animal->otherHerbivore)
		{
			Animal->otherHerbivore->IsBeingChased = false;
			Animal->otherHerbivore->Chaser = nullptr;
		}
		if (PreyBlackboard)
			PreyBlackboard->SetValueAsBool("TmpRunAway", false);
		Blackboard->SetValueAsBool("CanEat", false);
		Blackboard->SetValueAsVector("EatVector", Animal->GetActorLocation());
		Blackboard->SetValueAsBool("EatPrey", false);
	}
}