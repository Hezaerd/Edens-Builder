// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission1.h"

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
void UMission1::CheckMission(AStatsTracker* tracker)
{
	if (tracker->NumberOfHerbivores >= NumberSpawnedAnimals)
	{
		Succeeded = true;
		this->Succeed(0);
	}
	
}