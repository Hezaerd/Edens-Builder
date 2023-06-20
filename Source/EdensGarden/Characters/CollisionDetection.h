// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicAnimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Basic/Planet.h"
#include "CollisionDetection.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EDENSGARDEN_API UCollisionDetection : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCollisionDetection();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	ABasicAnimal* Character;

	UPROPERTY(VisibleAnywhere, Category = "Trigger")
		USphereComponent* CollisionSphere;


	APlanet* planet;

	AActor* LastActor;

	//Vector collision

	FVector currentlocation;

	FVector ForwardVector;
	FVector RightSideVector;
	FVector LeftSideVector;


	UPROPERTY(EditAnywhere)
		float VectorScale = 10;

private:

	void UpdateVectors();

	void VectorCollision();

	UBlackboardComponent* Blackboard;

}; 
