// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Interface/GrabbableInterface.h"

#include "GrabbableActor.generated.h"

UCLASS()
class EDENSGARDEN_API AGrabbableActor : public AActor, public IGrabbableInterface
{
	GENERATED_BODY()

public:
	AGrabbableActor();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* Base;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* InteractiveCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Informations")
		bool bIsHeld;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Informations")
		class USkeletalMeshComponent* CurrentMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Informations")
		bool bSimulatePhysics;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Informations")
		FVector DropLocation;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AHandsVR* Player;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		void Pickup(class USkeletalMeshComponent* MotionController);
	virtual void Pickup_Implementation(class USkeletalMeshComponent* MotionController) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		void Drop(class USkeletalMeshComponent* MotionController);
	virtual void Drop_Implementation(class USkeletalMeshComponent* MotionController) override;

	FVector StartLocation;
	FRotator StartRotation;

	class USoundWave* propellerAudioCue;
	class UAudioComponent* propellerAudioComponent;
	class USkeletalMeshComponent* HandMesh;
};
