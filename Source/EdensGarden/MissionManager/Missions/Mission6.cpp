// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission6.h"

void UMission6::CheckMission(AStatsTracker* tracker)
{
	if (tracker->CarnivoreAttacked)
	{
		Succeeded = true;
		this->Succeed(2);
	}


}