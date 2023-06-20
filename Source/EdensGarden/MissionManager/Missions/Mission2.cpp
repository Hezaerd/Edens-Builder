// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission2.h"

void UMission2::CheckMission(AStatsTracker* tracker)
{
	if (tracker->NumberOfVegetations >= NumberOfTrees)
	{
		Succeeded = true;
		this->Succeed(1);
	}

}