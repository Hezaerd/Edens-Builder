// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Mission.h"
#include "../../Statistics/StatsTracker.h"
#include "Mission1.generated.h"

/**
 * 
 */
UCLASS()
class EDENSGARDEN_API UMission1 : public UMission
{
	GENERATED_BODY()

	void CheckMission(AStatsTracker* tracker) override;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Numbers")
		int NumberSpawnedAnimals = 10;

};
