// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RotateTargetWithPlanet.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API URotateTargetWithPlanet : public UBTService
{
	GENERATED_BODY()
public:
	URotateTargetWithPlanet(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector NextDestination;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	FRotator StartRotation;

	class UBlackboardComponent* Blackboard;
};
