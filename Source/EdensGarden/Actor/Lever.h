// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableActor.h"

#include "Lever.generated.h"

UCLASS()
class EDENSGARDEN_API ALever : public AGrabbableActor
{
	GENERATED_BODY()
public:

	ALever();

	void Pickup(class USkeletalMeshComponent* MotionController);
	virtual void Pickup_Implementation(class USkeletalMeshComponent* MotionController) override;

	void Drop(class USkeletalMeshComponent* MotionController);
	virtual void Drop_Implementation(class USkeletalMeshComponent* MotionController) override;

	virtual void Tick(float DeltaTime) override;

	bool bIsHeld;

	virtual void BeginPlay() override;
public:

	FRotator StartRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* BlockBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Handle;

	class AHandsVR* Hands;

	class USkeletalMeshComponent* Motion;
};
