// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MissionManager/MissionManager.h"
#include "GameFramework/Actor.h"
#include "StatsTracker.generated.h"

UCLASS()
class EDENSGARDEN_API AStatsTracker : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStatsTracker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	int NumberOfEntities = 0;
	int NumberOfHerbivores = 0;
	int NumberOfCarnivores = 0;
	int NumberOfVegetations = 0;

	bool CarnivoreAttacked = false;

	TArray<class ACarnivore*> Carnivores;

	TArray<class AHerbivore*> Herbivores;

	AMissionManager* MissionManager;

	void AddEntity(ACharacter* Species);
};
