// Fill out your copyright notice in the Description page of Project Settings.


#include "ReproductionComp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
// Sets default values for this component's properties
UReproductionComp::UReproductionComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UReproductionComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UReproductionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	ABasicAnimal* animal = Cast<ABasicAnimal>(GetOwner());

	if (animal->ReproductionPoints < animal->MinReproductionPoints)
	{
		animal->ReproductionPoints += ReproductionTime * DeltaSeconds;
	}
}

