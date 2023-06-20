// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionScoreboard.h"
#include "../Player/HandsVR.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Containers/Array.h"
#include "Components/DecalComponent.h"
// Sets default values
AMissionScoreboard::AMissionScoreboard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	DecalTick = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DecalTick->SetupAttachment(Meshcomp);
	FString sPath = "/Game/Assets/Materials/M_BlueTick";
	TickMat = LoadMaterialFromPath(FName(*sPath));
	sPath = "/Game/Assets/Materials/M_NoTick";
	NoTickMat = LoadMaterialFromPath(FName(*sPath));
	DecalTick->SetMaterial(0, NoTickMat);
	Decals.Add(DecalTick);

	DecalTick2 = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal2"));
	DecalTick2->SetupAttachment(Meshcomp);
	DecalTick2->SetMaterial(0, NoTickMat);
	Decals.Add(DecalTick2);

	DecalTick3 = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal3"));
	DecalTick3->SetupAttachment(Meshcomp);
	DecalTick3->SetMaterial(0, NoTickMat);
	Decals.Add(DecalTick3);
}

// Called when the game starts or when spawned
void AMissionScoreboard::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMissionScoreboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissionScoreboard::FinishedMission(int numb)
{
	Decals[numb]->SetMaterial(0, TickMat);
}