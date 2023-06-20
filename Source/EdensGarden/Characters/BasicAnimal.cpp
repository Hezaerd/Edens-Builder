// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAnimal.h"
#include "../Basic/Planet.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "../AIC_BasicAnimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Math/UnrealMathUtility.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimSequence.h"
// Sets default values
ABasicAnimal::ABasicAnimal()
{
	PrimaryActorTick.bCanEverTick = false;
	age = 0;
	Health = 10;
	Damage = 0;
	AdultAge = 10;
	DeathAge = 60;
	NutritionalValue = 1;
	WalkSpeed = 1;
	RunSpeed = 2;
	EatSpeed = 1;
	Aggressivity = 1;
	HungerFactor = 1;
	IsMating = false;

	StopSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Stop Sphere"));
	StopSphere->InitSphereRadius(100);
	StopSphere->SetupAttachment(GetRootComponent());


	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	DetectionSphere->InitSphereRadius(500);
	DetectionSphere->SetupAttachment(GetRootComponent());


	bReplicates = true;


}

// Called when the game starts or when spawned
void ABasicAnimal::BeginPlay()
{


	Super::BeginPlay();
	planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));

	
}

// Called every frame
void ABasicAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsWalking)
	{
		if (PlayOnce)
		{
			this->GetMesh()->PlayAnimation(this->Walking, true);
			PlayOnce = false;
		}
	}
	else
	{
		PlayOnce = true;
	}
	IsWalking = false;
}

// Called to bind functionality to input
void ABasicAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


