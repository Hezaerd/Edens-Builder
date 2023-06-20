// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckForFoodCarnivore.generated.h"

/**
 *
 */
UCLASS()
class EDENSGARDEN_API UCheckForFoodCarnivore : public UBTService
{
	GENERATED_BODY()
public:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool PlayOnce = true;

};
