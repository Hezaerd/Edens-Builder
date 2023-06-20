// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Mission.h"
#include "../../Statistics/StatsTracker.h"
#include "Mission6.generated.h"

/**
 * 
 */
UCLASS()
class EDENSGARDEN_API UMission6 : public UMission
{
	GENERATED_BODY()
public:

	void CheckMission(AStatsTracker* tracker) override;
};
