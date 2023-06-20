// Fill out your copyright notice in the Description page of Project Settings.

#include "Planet.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../Actor/Lever.h"
// Sets default values
APlanet::APlanet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> levers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALever::StaticClass(), levers);

	if (levers.Num() > 0)
	{
		if (levers[0]->ActorHasTag("Horizontal"))
		{
			Horizontal = Cast<ALever>(levers[0]);
			Vertical = Cast<ALever>(levers[1]);
		}
		else if (levers[0]->ActorHasTag("Vertical"))
		{
			Horizontal = Cast<ALever>(levers[1]);
			Vertical = Cast<ALever>(levers[0]);
		}
	}
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Horizontal && Vertical)
	{
		if (Horizontal->Handle->GetRelativeRotation().Pitch != 0)
		{
			float pitch = Horizontal->Handle->GetRelativeRotation().Pitch;
			FQuat rot = FQuat(FRotator(0, 0, -pitch * GetWorld()->GetDeltaSeconds() * 2));
			this->AddActorWorldRotation(rot);
		}
		if (Vertical->Handle->GetRelativeRotation().Pitch != 0)
		{
			float yaw = Vertical->Handle->GetRelativeRotation().Pitch;
			FQuat rot = FQuat(FRotator(0, yaw * GetWorld()->GetDeltaSeconds() * 2, 0));

			this->AddActorWorldRotation(rot);
		}
	}
	if (ShouldRotate)
	{
		CurrentRotation = GetActorRotation();
		CurrentRotation.Yaw += DeltaTime * 10;
		SetActorRotation(CurrentRotation);
	}
}