// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HerbivoreAI.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API AHerbivoreAI : public AAIController
{
	GENERATED_BODY()

public:

	AHerbivoreAI(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	void OnPossess(APawn* const pawn) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BTree;

	class UBlackboardComponent* Blackboard;
};
