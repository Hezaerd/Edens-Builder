// Fill out your copyright notice in the Description page of Project Settings.

#include "Herbivore.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/SphereComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Animation/AnimSequence.h"
#include "../Statistics/StatsTracker.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
AHerbivore::AHerbivore()
{
	PrimaryActorTick.bCanEverTick = true;
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

	StopSphere->OnComponentBeginOverlap.AddDynamic(this, &AHerbivore::BeginOverlap);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHerbivore::BeginOverlap);

	StopSphere->OnComponentEndOverlap.AddDynamic(this, &AHerbivore::EndOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AHerbivore::EndOverlap);
	otherHerbivore = nullptr;

	bFirstSpawn = false;
}

void AHerbivore::BeginPlay()
{
	Super::BeginPlay();
	planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	bool bLoop = true;
	GetMesh()->PlayAnimation(Walking, true);

	// Niagara the Spawn VFX
	Tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));

	Tracker->Herbivores.Add(this);
}

void AHerbivore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHerbivore::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OverlappedComponent == StopSphere)
		{
			if (OtherActor->GetClass()->IsChildOf(APlanet::StaticClass()))
			{
				if (bFirstSpawn == false)
				{
					FVector Difference = this->GetActorLocation() - OtherActor->GetActorLocation();
					FRotator Rot = FRotationMatrix::MakeFromZ(Difference).Rotator();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX_Spawn, GetActorLocation(), Rot, FVector(1, 1, 1), true, true, ENCPoolMethod::AutoRelease);
					bFirstSpawn = true;
				}

				IsOnPlanet = true;
				this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
		else if (OverlappedComponent == DetectionSphere)
		{
			if (OtherActor->GetClass()->IsChildOf(AHerbivore::StaticClass()))
			{
				AHerbivore* tmp = Cast<AHerbivore>(OtherActor);
				if (HerbivoreType == tmp->HerbivoreType)
				{
					if (tmp->ReproductionPoints > 0)
					{
						if (otherHerbivores.Find(tmp) == INDEX_NONE)
						{
							otherHerbivores.Emplace(Cast<AHerbivore>(OtherActor));
						}
					}
				}
			}
			else if (OtherActor->GetClass()->IsChildOf(AVegetation::StaticClass()))
			{
				AVegetation* tmp = Cast<AVegetation>(OtherActor);
				if (Vegetations.Find(tmp) == INDEX_NONE && tmp->Health > 0)
				{
					Vegetations.Emplace(Cast<AVegetation>(OtherActor));
				}
			}
		}
	}
}

void AHerbivore::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OverlappedComponent == StopSphere)
		{
			if (OtherActor->GetClass()->IsChildOf(APlanet::StaticClass()))
			{
				IsOnPlanet = false;
				this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			}
		}
		else if (OverlappedComponent == DetectionSphere)
		{
			if (OtherActor->GetClass()->IsChildOf(AHerbivore::StaticClass()))
			{
				if (HerbivoreType == Cast<AHerbivore>(OtherActor)->HerbivoreType)
				{
					otherHerbivores.RemoveSingleSwap(Cast<AHerbivore>(OtherActor));
				}
			}
			else if (OtherActor->GetClass()->IsChildOf(AVegetation::StaticClass()))
			{
				Vegetations.RemoveSingleSwap(Cast<AVegetation>(OtherActor));
			}
		}
	}
}