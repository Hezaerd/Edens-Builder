// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionManager.h"
#include "Missions/Mission1.h"
#include "../Widget/MissionScoreboard.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../Statistics/StatsTracker.h"
// Sets default values
AMissionManager::AMissionManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMissionManager::BeginPlay()
{
	Super::BeginPlay();
	tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));
	//scoreboard = Cast<AMissionScoreboard>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionScoreboard::StaticClass()));

	for (int i = 0; i < Missions.Num(); ++i)
	{
		if (Missions[i]->IsChildOf(UMission::StaticClass()))
		{
			UMission* a = Cast<UMission>(Missions[i]->GetDefaultObject());
			if (i <= 2)
			{
				a->SetMissionScoreboard(scoreboard1);
			}
			else
			{
				a->SetMissionScoreboard(scoreboard2);
			}
		}
	}
}

// Called every frame
void AMissionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < Missions.Num(); ++i)
	{
		if (Missions[i]->IsChildOf(UMission::StaticClass()))
		{
			UMission* a = Cast<UMission>(Missions[i]->GetDefaultObject());
			a->CheckMission(tracker);
		}
	}
}