// Fill out your copyright notice in the Description page of Project Settings.

#include "Vegetation.h"
#include "../Basic/Planet.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "../Statistics/StatsTracker.h"
// Sets default values
AVegetation::AVegetation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVegetation::BeginPlay()
{
	Super::BeginPlay();
	Capsule = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (Capsule)
	{
		//RespawnSphere->OnComponentBeginOverlap.AddDynamic(this, &AVegetation::BeginOverlap);
		//RespawnSphere->OnComponentEndOverlap.AddDynamic(this, &AVegetation::EndOverlap);
		//RespawnSphere->OnComponentHit.AddDynamic(this, &AVegetation::Hit);
		//MinSpawnRadius->OnComponentBeginOverlap.AddDynamic(this, &AVegetation::BeginOverlap);
		mesh->OnComponentBeginOverlap.AddDynamic(this, &AVegetation::BeginOverlap);
	}
	planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	AStatsTracker* tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));
	if (tracker)
		tracker->AddEntity(this);
}

// Called every frame
void AVegetation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugSphere(GetWorld(), this->GetActorLocation(), RespawnSphere->GetScaledSphereRadius(), 8, FColor::Yellow);
	if (Radius < MaxRadius)
	{
		Radius += 10 * DeltaTime;
	}

	if (SpawnPoints >= MaxSpawnPoints && CanSpawn)
	{
		SpawnOtherTree();
	}
	else
	{
		SpawnPoints += SpawnTime * DeltaTime;
	}

	//if (Grasses < MaxGrass * RespawnSphere->GetScaledSphereRadius())
	//{
	//	//SpawnVegetation();
	//}
	if (Health < 0)
	{
		for (int i = 0; i < GrassArray.Num(); ++i)
		{
			GrassArray[i]->Destroy();
		}
		AStatsTracker* tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));
		if (tracker)
			tracker->NumberOfVegetations -= 1;
		Destroy();
	}
}

void AVegetation::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OverlappedComponent == mesh)
		{
			if (OtherActor->GetClass()->IsChildOf(APlanet::StaticClass()))
			{
				IsOnPlanet = true;
				this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

void AVegetation::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OverlappedComponent == mesh)
		{
			if (OtherActor->GetClass()->IsChildOf(APlanet::StaticClass()))
			{
				IsOnPlanet = false;
				this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

void AVegetation::Hit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (HitComponent == mesh)
		{
			if (OtherActor->GetClass()->IsChildOf(APlanet::StaticClass()))
			{
				IsOnPlanet = true;
				this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
			}
			if (OtherActor->GetClass()->IsChildOf(AVegetation::StaticClass()))
			{
				if (age > Cast<AVegetation>(OtherActor)->age)
				{
					AStatsTracker* tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));
					if (tracker)
						tracker->NumberOfVegetations -= 1;
					OtherActor->Destroy();
				}
				else
				{
					AStatsTracker* tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));
					if (tracker)
						tracker->NumberOfVegetations -= 1;
					Destroy();
				}
			}
		}
	}
}

void AVegetation::SpawnOtherTree()
{
	FRandomStream rand;
	rand.GenerateNewSeed();

	FVector RandomPoint = rand.GetUnitVector();

	RandomPoint *= Radius;
	RandomPoint += this->GetActorLocation();

	FVector distanceVec = RandomPoint - planet->GetActorLocation();
	float distance = distanceVec.Length();
	distanceVec.Normalize();
	USphereComponent* PlanetSphere = Cast<USphereComponent>(planet->GetComponentByClass(USphereComponent::StaticClass()));

	FVector surfacepos = planet->GetActorLocation() + distanceVec * PlanetSphere->GetScaledSphereRadius();
	if (FVector::Distance(surfacepos, GetActorLocation()) < RespawnDifference)
	{
		return;
	}
	SpawnPoints = 0;
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Plant/Tree.Tree")));
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	//Check if tree in position
	AVegetation* newVegetation = GetWorld()->SpawnActor<AVegetation>(GeneratedBP->GeneratedClass, surfacepos, this->GetActorRotation(), SpawnParams);
	CanSpawn = false;
}

void AVegetation::SpawnVegetation()
{
	FRandomStream rand;
	rand.GenerateNewSeed();

	FVector RandomPoint = rand.GetUnitVector();

	RandomPoint *= Radius;
	RandomPoint += this->GetActorLocation();

	FVector distanceVec = RandomPoint - planet->GetActorLocation();
	float distance = distanceVec.Length();
	distanceVec.Normalize();
	USphereComponent* PlanetSphere = Cast<USphereComponent>(planet->GetComponentByClass(USphereComponent::StaticClass()));

	FVector surfacepos = planet->GetActorLocation() + distanceVec * PlanetSphere->GetScaledSphereRadius() * 0.95f;

	AStaticMeshActor* MyNewActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
	MyNewActor->SetMobility(EComponentMobility::Movable);
	MyNewActor->SetActorLocation(surfacepos);
	MyNewActor->SetActorRotation(this->GetActorRotation());
	MyNewActor->AttachToActor(planet, FAttachmentTransformRules::KeepWorldTransform);
	UStaticMeshComponent* MeshComponent = MyNewActor->GetStaticMeshComponent();
	if (MeshComponent)
	{
		MeshComponent->SetStaticMesh(Grass);
	}

	GrassArray.Add(MyNewActor);
	Grasses += 1;
}