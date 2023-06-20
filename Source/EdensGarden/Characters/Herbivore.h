// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicAnimal.h"
#include "../Basic/Planet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Vegetation.h"
#include "Herbivore.generated.h"

UENUM()
namespace EHerbivoreList {
	enum Type {
		BUNNY				UMETA(DisplayName = "Bunny"),
		DEER				UMETA(DisplayName = "Deer"),
		ELEPHANT			UMETA(DisplayName = "Elephant")
	};
}


UCLASS()
class EDENSGARDEN_API AHerbivore : public ABasicAnimal
{
	GENERATED_BODY()

public:
	AHerbivore();

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	class AStatsTracker* Tracker;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UBlackboardComponent* blackboard;


	//Colliding actor
	AHerbivore* otherHerbivore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
	TArray<AHerbivore*> otherHerbivores;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
	TArray<AVegetation*> Vegetations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimalType")
		TEnumAsByte<EHerbivoreList::Type> HerbivoreType;


private:

	APlanet* planet;

	bool bFirstSpawn;
	
};
