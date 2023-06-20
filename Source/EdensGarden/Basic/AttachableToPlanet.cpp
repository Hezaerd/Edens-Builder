// Fill out your copyright notice in the Description page of Project Settings.

#include "AttachableToPlanet.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Math/RotationMatrix.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BrainComponent.h"
// Sets default values for this component's properties
UAttachableToPlanet::UAttachableToPlanet()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	GravitySpeed = 4;
}

// Called when the game starts
void UAttachableToPlanet::BeginPlay()
{
	Super::BeginPlay();
	planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	owner = Cast<ABasicObject>(GetOwner());
	owner->GetMesh()->SetEnableGravity(false);
	StartDistance = 0;
}

// Called every frame
void UAttachableToPlanet::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (planet)
	{
		GetLocationSurfaceNormal();

		MoveTowardsSpot();
		FVector Difference = owner->GetActorLocation() - position;
		FRotator Rot = FRotationMatrix::MakeFromZ(Difference).Rotator();
		if (ShouldRotate)
			owner->SetActorRotation(Rot);
	}
}

void UAttachableToPlanet::GetLocationSurfaceNormal()
{
	FVector differenceVec = owner->GetActorLocation() - planet->GetActorLocation();
	differenceVec.Normalize();
	Normal = differenceVec;

	differenceVec *= planet->GetActorScale().Length();

	position = planet->GetActorLocation();
}

void UAttachableToPlanet::MoveTowardsSpot()
{
	FVector distanceVec = owner->GetActorLocation() - position;
	float distance = distanceVec.Length();
	distanceVec.Normalize();
	USphereComponent* a = Cast<USphereComponent>(planet->GetComponentByClass(USphereComponent::StaticClass()));

	FVector surfacepos = position + distanceVec * a->GetScaledSphereRadius();

	//DrawDebugLine(GetWorld(), owner->GetActorLocation(), surfacepos, FColor(255, 0, 0));
	UCharacterMovementComponent* movement = owner->GetCharacterMovement();

	float vecdistance = (surfacepos - owner->GetActorLocation()).Length();
	if (StartDistance == 0)
	{
		StartDistance = vecdistance;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Name %f"), vecdistance));
	FVector impulse = surfacepos - owner->GetActorLocation();
	FVector Impulse2 = planet->GetActorLocation() - surfacepos;

	Impulse2.Normalize();
	impulse.Normalize();
	if (vecdistance > 5)
	{
		FVector loc = owner->GetActorLocation() + impulse;
		owner->SetActorLocation(loc);
	}
	if (ShouldRotate && !owner->IsOnPlanet)
	{
		movement->AddForce(Impulse2);
		IsAttached = false;
	}
	else if (ShouldRotate && owner->IsOnPlanet)
	{
		bWasOnPlanet = true;
		movement->Deactivate();

		IsAttached = true;
	}
	else if (!owner->IsOnPlanet)
	{
		movement->AddForce(Impulse2);

		if (bWasOnPlanet)
		{
			//movement->AddForce(Impulse2);
			//bWasOnPlanet = false;
			//owner->IsOnPlanet = true;
			//IsAttached = false;
			return;
		}
		if (UAIBlueprintHelperLibrary::GetAIController(owner)->GetBrainComponent())
			UAIBlueprintHelperLibrary::GetAIController(owner)->GetBrainComponent()->StopLogic("Not on planet");
		movement->AddForce(Impulse2);

		IsAttached = false;
	}
	else if (owner->IsOnPlanet)
	{
		if (!bWasOnPlanet)
		{
			movement->AddImpulse(Impulse2);
		}
		bWasOnPlanet = true;
		if (movement->Velocity == FVector::Zero()) // top of planet
		{
			movement->AddImpulse(owner->GetActorForwardVector());
			//owner->IsOnPlanet = false;
			//IsAttached = false;
			return;
		}

		IsAttached = true;
		if (UAIBlueprintHelperLibrary::GetAIController(owner)->GetBrainComponent())
			UAIBlueprintHelperLibrary::GetAIController(owner)->GetBrainComponent()->StartLogic();
	}
}