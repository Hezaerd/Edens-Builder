// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission.h"
#include "../Widget/MissionScoreboard.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
void UMission::Succeed(int numb)
{
	//Show Ui stuff here
	MissionScoreboard->FinishedMission(numb);

}
void UMission::SetMissionScoreboard(class AMissionScoreboard* scoreboard)
{
	MissionScoreboard = scoreboard;
}
