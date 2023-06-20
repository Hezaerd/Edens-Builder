// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicAnimal.h"
#include "../Basic/Planet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Vegetation.h"
#include "Herbivore.h"
#include "Carnivore.generated.h"

UENUM()
namespace ECarnivoreList {
	enum Type {
		WOLF				UMETA(DisplayName = "Wolf"),
		KRAKEN				UMETA(DisplayName = "Kraken"),
		BEAR				UMETA(DisplayName = "Bear"),
		LION				UMETA(DisplayName = "Lion"),
		SHARK				UMETA(DisplayName = "Shark")
	};
}
UCLASS()
class EDENSGARDEN_API ACarnivore : public ABasicAnimal
{
	GENERATED_BODY()

public:
	ACarnivore();

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UBlackboardComponent* blackboard;

	//Colliding actor
	ACarnivore* otherCarnivore;
	ACarnivore* otherEatableCarnivore;
	AHerbivore* otherHerbivore;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		TArray<ACarnivore*> otherCarnivores;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		TArray<ACarnivore*> EatableCarnivores;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		TArray<AHerbivore*> Herbivores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimalType")
		TEnumAsByte<ECarnivoreList::Type> CarnivoreType;

	//Mission Variables
	int NumberOfKills = 0;
	int NumberOfBunnyKills = 0;
private:
	class AStatsTracker* Tracker;
	APlanet* planet;

	bool bFirstSpawn;
};
