// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission5.h"

void UMission5::CheckMission(AStatsTracker* tracker)
{
	if (tracker->NumberOfVegetations == 0)
	{
		Succeeded = true;
		this->Succeed(1);
	}


}