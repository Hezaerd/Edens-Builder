// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrabbableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGrabbableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EDENSGARDEN_API IGrabbableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void Pickup(class USkeletalMeshComponent*MotionController);
		virtual void Pickup_Implementation(class USkeletalMeshComponent* MotionController);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void Drop(class USkeletalMeshComponent* MotionController);
		virtual void Drop_Implementation(class USkeletalMeshComponent* MotionController);
};
