// Fill out your copyright notice in the Description page of Project Settings.

#include "Lever.h"
#include "Components/StaticMeshComponent.h"
#include "../Player/HandsVR.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"
ALever::ALever()
{
	BlockBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blockbase"));
	BlockBase->SetupAttachment(GetRootComponent());

	Handle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	Handle->SetupAttachment(GetRootComponent());
}
void ALever::BeginPlay()
{
	Super::BeginPlay();

	StartRotation = Handle->GetComponentRotation();
	TArray<AActor*> actors;
}
void ALever::Pickup_Implementation(class USkeletalMeshComponent* MotionController)
{
	bIsHeld = true;
	Motion = MotionController;
}

void ALever::Drop_Implementation(class USkeletalMeshComponent* MotionController)
{
	bIsHeld = false;
	Motion = MotionController;
}

void ALever::Tick(float DeltaTime)
{
	if (bIsHeld)
	{
		FVector location = Handle->GetComponentLocation();
		FRotator rotation = Handle->GetComponentRotation();
		FVector Scale = Handle->GetComponentScale();
		FTransform newHandletans = UKismetMathLibrary::MakeTransform(location, rotation, Scale);
		FTransform handstrans = Motion->GetComponentTransform();

		//FTransform handstrans = hands->GetActorTransform();
		FTransform AdditionTransform = UKismetMathLibrary::MakeRelativeTransform(handstrans, newHandletans);

		FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(), AdditionTransform.GetLocation());

		float newPitch = PlayerRot.Pitch + rotation.Pitch;
		float ClampedFloat = FMath::ClampAngle(newPitch, -45, 45);

		FRotator newRotation = FRotator(ClampedFloat, 0, 0);

		Handle->SetRelativeRotation(newRotation);
	}
	else
	{
		if (Handle->GetRelativeRotation().Pitch > 0)
		{
			FQuat delta = FQuat(GetWorld()->GetDeltaSeconds() * 2, 0, 0, 0);
			Handle->SetRelativeRotation(StartRotation);
		}
		else if (Handle->GetRelativeRotation().Pitch < 0)
		{
			FQuat delta = FQuat(-GetWorld()->GetDeltaSeconds() * 2, 0, 0, 0);
			Handle->SetRelativeRotation(StartRotation);
		}
	}
}