// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MotionControllerComponent.h"
#include "../Basic/Planet.h"
#include "HandsVR.generated.h"

UCLASS()
class EDENSGARDEN_API AHandsVR : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHandsVR();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	// Left Motion Controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionController")
	class UMotionControllerComponent* LeftMotionController;

	// Right Motion Controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionController")
	class UMotionControllerComponent* RightMotionController;

	// left hand skeletical mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* LeftHandMesh;

	// Right hand skeletical mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MotionController")
	class USkeletalMeshComponent* RightHandMesh;

	// left hand grab sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionController")
	class USphereComponent* LeftDropSphere;

	// Right hand grab sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionController")
	class USphereComponent* RightDropSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragNDrop")
	bool bCanDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragNDrop")
	bool bIsHolding;



	UFUNCTION(BlueprintCallable)
		void GrabSomethingLeft(class UMotionControllerComponent* MotionController);

	UFUNCTION(BlueprintCallable)
	void GrabSomethingRight(class UMotionControllerComponent* MotionController);

	UFUNCTION(BlueprintCallable)
		void DropSomethingLeft(class UMotionControllerComponent* MotionController);

	UFUNCTION(BlueprintCallable)
		void DropSomethingRight(class UMotionControllerComponent* MotionController);

	class AGrabbableActor* PlanetModel;

	FRotator StartRotation;

	FVector StartVector;
	AActor* center;
	APlanet* planet;
	AActor* GrabbedActorLeft;
	AActor* GrabbedActorRight;
};
