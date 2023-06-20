// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Planet.generated.h"

UCLASS()
class EDENSGARDEN_API APlanet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FRotator CurrentRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
		bool ShouldRotate = false;

	class ALever* Horizontal;
	class ALever* Vertical;
};
