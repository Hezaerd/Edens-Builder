// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Basic/Planet.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_SelectRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class EDENSGARDEN_API UBTTask_SelectRandomPoint : public UBTTaskNode
{
	GENERATED_BODY()


public:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	

	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector NextDestination;


private:

	APlanet* planet;

	bool PlayOnce = true;
};
