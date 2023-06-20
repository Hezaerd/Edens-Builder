// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "EatHerbivore.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API UEatHerbivore : public UBTTaskNode
{
	GENERATED_BODY()
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere Colliders")
		bool IsCarnivore = false;

	bool PlayOnce = true;
};
