// Fill out your copyright notice in the Description page of Project Settings.

#include "Mission4.h"
#include "../../Characters/Herbivore.h"
void UMission4::CheckMission(AStatsTracker* tracker)
{
	for (int i = 0; i < tracker->Herbivores.Num(); ++i)
	{
		if (tracker->Herbivores[i]->Generation >= 5)
		{
			Succeeded = true;
			this->Succeed(0);
		}
	}
}