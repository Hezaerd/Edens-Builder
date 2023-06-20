// Fill out your copyright notice in the Description page of Project Settings.

#include "Carnivore.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/SphereComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Animation/AnimSequence.h"
#include "../Statistics/StatsTracker.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
ACarnivore::ACarnivore()
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

	bFirstSpawn = false;

	StopSphere->OnComponentBeginOverlap.AddDynamic(this, &ACarnivore::BeginOverlap);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACarnivore::BeginOverlap);

	StopSphere->OnComponentEndOverlap.AddDynamic(this, &ACarnivore::EndOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ACarnivore::EndOverlap);
	otherCarnivore = nullptr;
}

void ACarnivore::BeginPlay()
{
	Super::BeginPlay();
	planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	bool bLoop = true;
	GetMesh()->PlayAnimation(Walking, true);
	Tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));

	Tracker->Carnivores.Add(this);
}

void ACarnivore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACarnivore::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
					UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->VFX_Spawn, this->GetActorLocation(), Rot, FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);
					bFirstSpawn = true;
				}

				IsOnPlanet = true;
				this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
		else if (OverlappedComponent == DetectionSphere)
		{
			if (OtherActor->GetClass()->IsChildOf(ACarnivore::StaticClass()))
			{
				ACarnivore* tmp = Cast<ACarnivore>(OtherActor);
				if (CarnivoreType == tmp->CarnivoreType)
				{
					if (tmp->ReproductionPoints > 0)
					{
						if (otherCarnivores.Find(tmp) == INDEX_NONE)
						{
							otherCarnivores.Emplace(Cast<ACarnivore>(OtherActor));
						}
					}
				}
				else
				{
					if (tmp->ReproductionPoints > 0)
					{
						if (EatableCarnivores.Find(tmp) == INDEX_NONE)
						{
							EatableCarnivores.Emplace(Cast<ACarnivore>(OtherActor));
						}
					}
				}
			}
			else if (OtherActor->GetClass()->IsChildOf(AHerbivore::StaticClass()))
			{
				AHerbivore* tmp = Cast<AHerbivore>(OtherActor);

				if (Herbivores.Find(tmp) == INDEX_NONE)
				{
					Herbivores.Emplace(Cast<AHerbivore>(OtherActor));
				}
			}
		}
	}
}

void ACarnivore::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
			if (OtherActor->GetClass()->IsChildOf(ACarnivore::StaticClass()))
			{
				if (CarnivoreType == Cast<ACarnivore>(OtherActor)->CarnivoreType)
					otherCarnivores.RemoveSingleSwap(Cast<ACarnivore>(OtherActor));
				else
				{
					EatableCarnivores.RemoveSingleSwap(Cast<ACarnivore>(OtherActor));
				}
			}
			else if (OtherActor->GetClass()->IsChildOf(AHerbivore::StaticClass()))
			{
				AHerbivore* tmp = Cast<AHerbivore>(OtherActor);

				tmp->IsBeingChased = false;

				tmp->Chaser = nullptr;

				Herbivores.RemoveSingleSwap(Cast<AHerbivore>(OtherActor));
			}
		}
	}
}