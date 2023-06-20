// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckAroundCarnivore.generated.h"

UCLASS()
class EDENSGARDEN_API UCheckAroundCarnivore : public UBTService
{
	GENERATED_BODY()

		void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:

	bool PlayOnce = true;
};
