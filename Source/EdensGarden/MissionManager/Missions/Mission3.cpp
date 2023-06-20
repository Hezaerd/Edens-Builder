// Fill out your copyright notice in the Description page of Project Settings.

#include "Mission3.h"
#include "../../Statistics/StatsTracker.h"
#include "../../Characters/Carnivore.h"
void UMission3::CheckMission(AStatsTracker* tracker)
{
	for (int i = 0; i < tracker->Carnivores.Num(); ++i)
	{
		if (tracker->Carnivores[i]->NumberOfBunnyKills >= BunniesToKill)
		{
			Succeeded = true;
			this->Succeed(2);
		}
	}
}