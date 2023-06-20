// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicObject.h"
#include "Components/CapsuleComponent.h"
#include "Vegetation.generated.h"

UCLASS()
class EDENSGARDEN_API AVegetation : public ABasicObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVegetation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCapsuleComponent* Capsule;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Hit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float MaxSpawnPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grass")
		class UStaticMesh* Grass;

	void SpawnOtherTree();
	void SpawnVegetation();

	int MaxGrass = 5;
	float Radius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RespawnRadius")
		float MaxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RespawnRadius")
		float RespawnDifference;

private:

	int Grasses = 0;
	class APlanet* planet;
	TArray<class AStaticMeshActor*> GrassArray;

	class UStaticMeshComponent* mesh;

	bool CanSpawn = true;
};
