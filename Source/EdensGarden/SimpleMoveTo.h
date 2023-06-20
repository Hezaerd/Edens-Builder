// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AIC_BasicAnimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Basic/Planet.h"
#include "SimpleMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class EDENSGARDEN_API USimpleMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector NextDestination;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	float MinRadius = 100.f;
	
	FVector TmpPoint;
	void RotateLeft();
	void RotateRight();

	float RotationSpeed = 100;
	UPROPERTY(EditAnywhere)
		float VectorScale = 10;

	UPROPERTY(EditAnywhere)
		bool ActiveCollision;
	UPROPERTY(EditAnywhere)
		bool IsRunning;

private:

	AAIController* cont;

	UBlackboardComponent* Blackboard;
	FVector lastlocation;

	APlanet* planet;



	ACharacter* Character;

	AAIC_BasicAnimal* AnimalAI;
};
