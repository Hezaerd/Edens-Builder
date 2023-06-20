// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "HerbivoreEatVegetation.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API UHerbivoreEatVegetation : public UBTTaskNode
{
	GENERATED_BODY()
public:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	class AHerbivore* Character;

	class AVegetation* Vegetation;

private:

	bool IsDoneEating = false;
};
