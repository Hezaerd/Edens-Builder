// Fill out your copyright notice in the Description page of Project Settings.


#include "HandsVR.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "../Interface/GrabbableInterface.h"
#include "../Actor/GrabbableActor.h"
#include "GameFramework/SpringArmComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AHandsVR::AHandsVR()
{
	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetTrackingSource(EControllerHand::Left);
	LeftMotionController->SetupAttachment(RootComponent);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	LeftMotionController->SetTrackingSource(EControllerHand::Right);
	RightMotionController->SetupAttachment(RootComponent);

	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftMotionController);

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightMotionController);

	// Attach the camera to the camera boom
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);

	// The player can't drop
	bCanDrop = false;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHandsVR::BeginPlay()
{
	Super::BeginPlay();
	planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));




	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),"Center", actors);



	if (actors.Num() > 0)
	{
		center = actors[0];

		this->AttachToActor(center, FAttachmentTransformRules::KeepWorldTransform);
	}


}

// Called every frame
void AHandsVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (center)
	{
		FRotator newrot = (center->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(newrot);
	}
}

void AHandsVR::GrabSomethingLeft(UMotionControllerComponent* MotionController)
{
	FVector WorldPosition = MotionController->GetComponentLocation()+ MotionController->GetForwardVector() * 3;
	FVector Foward = MotionController->GetComponentLocation() + MotionController->GetForwardVector() * 6;
	FHitResult HitResult;
	if (UKismetSystemLibrary::BoxTraceSingle(this, WorldPosition, Foward, FVector(2, 2, 2), FRotator(0, 0, 0), ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, -1))
	{
		if (UKismetSystemLibrary::IsValid(HitResult.GetActor()))
		{
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()))
			{
				IGrabbableInterface::Execute_Pickup(HitResult.GetActor(), LeftHandMesh);

				GrabbedActorLeft = HitResult.GetActor();

				return;
			}
		}
	}


}

void AHandsVR::GrabSomethingRight(UMotionControllerComponent* MotionController)
{
	FVector WorldPosition = MotionController->GetComponentLocation() + MotionController->GetForwardVector() * 3;
	FVector Foward = MotionController->GetComponentLocation() + MotionController->GetForwardVector() * 6;
	FHitResult HitResult;
	if (UKismetSystemLibrary::BoxTraceSingle(this, WorldPosition, Foward, FVector(2, 2, 2), FRotator(0, 0, 0), ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, -1))
	{
		if (UKismetSystemLibrary::IsValid(HitResult.GetActor()))
		{
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()))
			{
				IGrabbableInterface::Execute_Pickup(HitResult.GetActor(), RightHandMesh);

				GrabbedActorRight = HitResult.GetActor();

				return;
			}
		}
	}
	{
		if (UKismetSystemLibrary::IsValid(HitResult.GetActor()))
		{
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(UGrabbableInterface::StaticClass()))
			{
				IGrabbableInterface::Execute_Pickup(HitResult.GetActor(), RightHandMesh);

				GrabbedActorRight = HitResult.GetActor();
				return;
			}
		}
	}
}

void AHandsVR::DropSomethingLeft(UMotionControllerComponent* MotionController)
{
	if (GrabbedActorLeft)
	{
		IGrabbableInterface::Execute_Drop(GrabbedActorLeft, LeftHandMesh);
	}

}
void AHandsVR::DropSomethingRight(class UMotionControllerComponent* MotionController)
{
	if (GrabbedActorRight)
	{
		IGrabbableInterface::Execute_Drop(GrabbedActorRight, RightHandMesh);
	}

}