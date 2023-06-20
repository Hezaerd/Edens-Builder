// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicObject.generated.h"

UCLASS()
class EDENSGARDEN_API ABasicObject : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int AdultAge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int DeathAge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int NutritionalValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int HungerFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int MaxHunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float CurrentHunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int HungerThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float MaxScale;

	bool IsDead;
	bool IsOnPlanet;
};
