// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MateCarnivores.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API UMateCarnivores : public UBTTaskNode
{
	GENERATED_BODY()
public:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		TSubclassOf<AActor> ChildClass;

private:

	class UBlackboardComponent* Blackboard;
	class AAIController* Ai;
};
