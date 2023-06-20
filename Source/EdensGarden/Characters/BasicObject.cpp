// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicObject.h"

// Sets default values
ABasicObject::ABasicObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABasicObject::BeginPlay()
{

	Super::BeginPlay();
	FBodyInstance* BodyInst = GetMesh()->GetBodyInstance();

	if (!BodyInst) return;

	BodyInst->MassScale = 100;
	BodyInst->UpdateMassProperties();
	IsOnPlanet = false;
}

// Called every frame
void ABasicObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

