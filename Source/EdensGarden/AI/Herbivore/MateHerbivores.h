// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MateHerbivores.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API UMateHerbivores : public UBTTaskNode
{
	GENERATED_BODY()

public:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		TSubclassOf<AActor> ChildClass;

private:

	class UBlackboardComponent* Blackboard;
	class AAIController* Ai;
	bool PlayOnce = true;
};
