// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckForFoodHerbivore.h"
#include "AIController.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/SphereComponent.h"
#include "../../Basic/Planet.h"
#include "../../Characters/Herbivore.h"
#include "../../Characters/Vegetation.h"

void UCheckForFoodHerbivore::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	if (!Blackboard->GetValueAsBool("CanMate") && !Blackboard->GetValueAsBool("CanEat"))
	{
		AVegetation* ClosestVegetation = nullptr;
		double ClosestDistance = 0;
		for (int i = 0; i < Animal->Vegetations.Num(); ++i)
		{
			double distance = FVector::Dist(Animal->Vegetations[i]->GetActorLocation(), Animal->GetActorLocation());
			if ((distance < ClosestDistance || ClosestDistance == 0 && !Animal->IsDead))
			{
				ClosestDistance = distance;
				ClosestVegetation = Animal->Vegetations[i];
			}
		}
		if (!ClosestVegetation)
		{
			return;
		}
		if (Animal->CurrentHunger < 0.01 * Animal->HungerThreshold * Animal->MaxHunger && ClosestVegetation->Health > 0)
		{
			Blackboard->SetValueAsBool("CanEat", true);
			APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
			FVector DistanceVec = ClosestVegetation->GetActorLocation() - Planet->GetActorLocation();
			DistanceVec.Normalize();
			USphereComponent* PlanetSphere = Cast<USphereComponent>(Planet->GetComponentByClass(USphereComponent::StaticClass()));

			FVector SurfacePos = Planet->GetActorLocation() + DistanceVec * PlanetSphere->GetScaledSphereRadius();

			Blackboard->SetValueAsVector("EatVector", SurfacePos);
			Blackboard->SetValueAsObject("EatingVegetation", ClosestVegetation);


		}
		else
		{
			Blackboard->SetValueAsBool("CanEat", false);
		}
	}
}