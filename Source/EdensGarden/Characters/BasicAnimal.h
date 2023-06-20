// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Basic/Planet.h"
#include "BasicObject.h"
#include "BasicAnimal.generated.h"

UCLASS()
class EDENSGARDEN_API ABasicAnimal : public ABasicObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int EatSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		int Aggressivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float ReproductionPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BASIC VALUES")
		float MinReproductionPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere Colliders")
		class USphereComponent* StopSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere Colliders")
		class USphereComponent* DetectionSphere;

	//Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimSequence* Walking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimSequence* Running;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimSequence* Mating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		class UAnimSequence* Death;
	//Effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		class UNiagaraSystem* VFX_Angry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		class UNiagaraSystem* VFX_Death;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		class UNiagaraSystem* VFX_Horny;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
		class UNiagaraSystem* VFX_Spawn;


	bool IsMating;

	bool IsBeingChased;
	bool IsWalking = true;
	bool PlayOnce = true;
	ABasicAnimal* Chaser;
	int Generation = 1;
private:

	APlanet* planet;
};
