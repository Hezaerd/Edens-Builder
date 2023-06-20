// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Mission.h"
#include "../../Statistics/StatsTracker.h"
#include "Mission2.generated.h"

/**
 * 
 */
UCLASS()
class EDENSGARDEN_API UMission2 : public UMission
{
	GENERATED_BODY()

	void CheckMission(AStatsTracker* tracker) override;
public:


	int NumberOfTrees = 10;


};
