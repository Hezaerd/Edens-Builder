// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsTracker.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../Characters/Herbivore.h"
#include "../Characters/Carnivore.h"
#include "../Characters/Vegetation.h"
// Sets default values
AStatsTracker::AStatsTracker()
{
	
}

// Called when the game starts or when spawned
void AStatsTracker::BeginPlay()
{
	Super::BeginPlay();
	MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));
}

void AStatsTracker::AddEntity(ACharacter* Species)
{
	NumberOfEntities += 1;
	if (Species->IsA(AHerbivore::StaticClass()))
	{
		NumberOfHerbivores += 1;
	}
	else if (Species->IsA(ACarnivore::StaticClass()))
	{
		NumberOfCarnivores += 1;
	}
	else if (Species->IsA(AVegetation::StaticClass()))
	{
		NumberOfVegetations += 1;
	}
}