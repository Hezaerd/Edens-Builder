// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DamageHerbivore.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API UDamageHerbivore : public UBTTaskNode
{
	GENERATED_BODY()
public:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsCarnivore = false;

	class AStatsTracker* Tracker;

	bool PlayOnce = true;
};
