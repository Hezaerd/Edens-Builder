// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Mission.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class EDENSGARDEN_API UMission : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		FString MissionName;

	virtual void CheckMission(class AStatsTracker* tracker) {};
	
	void SetMissionScoreboard(class AMissionScoreboard* scoreboard);

	bool Succeeded;
	
protected:

	void Succeed(int numb);

	class AMissionScoreboard* MissionScoreboard;
};

