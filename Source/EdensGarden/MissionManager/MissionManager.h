// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mission.h"

#include "MissionManager.generated.h"
UCLASS()
class EDENSGARDEN_API AMissionManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMissionManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		TArray<TSubclassOf<UMission>> Missions;

	class AStatsTracker* tracker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		class AMissionScoreboard* scoreboard1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		class AMissionScoreboard* scoreboard2;
};
