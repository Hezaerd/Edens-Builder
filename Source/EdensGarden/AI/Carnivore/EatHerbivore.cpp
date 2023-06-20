// Fill out your copyright notice in the Description page of Project Settings.

#include "EatHerbivore.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../../Characters/Carnivore.h"
#include "../../Characters/Herbivore.h"
#include "../../Characters/BasicAnimal.h"

EBTNodeResult::Type UEatHerbivore::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	ACarnivore* Animal = Cast<ACarnivore>(owner_comp.GetAIOwner()->GetCharacter());

	UBlackboardComponent* Blackboard = owner_comp.GetBlackboardComponent();
	ABasicAnimal* Prey;
	if (IsCarnivore)
	{
		Prey = Animal->otherEatableCarnivore;
	}
	else
	{
		Prey = Animal->otherHerbivore;
	}

	if (!Prey)
	{
		Blackboard->SetValueAsBool("EatPrey", false);
		Blackboard->SetValueAsBool("EatPreyCarnivore", false);
		Blackboard->SetValueAsBool("CanEat", false);
		Animal->GetMesh()->PlayAnimation(Animal->Walking, true);
		return EBTNodeResult::Succeeded;
	}

	Animal->CurrentHunger += Prey->NutritionalValue;

	if (Animal->CurrentHunger > Animal->MaxHunger)
	{
		Animal->CurrentHunger = Animal->MaxHunger;
	}

	Prey->NutritionalValue = 0;
	if (IsCarnivore)
	{
		Animal->EatableCarnivores.RemoveSingleSwap(Animal->otherEatableCarnivore);
		Animal->otherEatableCarnivore = nullptr;
	}
	else
	{
		if (Animal->otherHerbivore->HerbivoreType == EHerbivoreList::Type::BUNNY)
		{
			Animal->NumberOfBunnyKills++;
		}

		Animal->Herbivores.RemoveSingleSwap(Animal->otherHerbivore);
		Animal->otherHerbivore = nullptr;
	}
	Animal->NumberOfKills++;
	Prey->Destroy();
	Blackboard->SetValueAsBool("EatPrey", false);
	Blackboard->SetValueAsBool("CanEat", false);
	Blackboard->SetValueAsBool("EatPreyCarnivore", false);
	Animal->GetMesh()->PlayAnimation(Animal->Walking, true);
	return EBTNodeResult::Succeeded;
}