// Fill out your copyright notice in the Description page of Project Settings.
#include "LifeComponent.h"
#include "../Basic/AttachableToPlanet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "Animation/AnimSequence.h"
#include "../Characters/BasicAnimal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../Basic/Planet.h"
// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();

	Animal = Cast<ABasicObject>(GetOwner());
	LiveAnimal = Cast<ABasicAnimal>(GetOwner());
	// ...
}

// Called every frame
void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	if (Animal)
	{
		Animal->age += DeltaSeconds * AgeTime;

		if (Animal->Health < 0)
		{
			if (UAIBlueprintHelperLibrary::GetAIController(Animal))
				if (UAIBlueprintHelperLibrary::GetAIController(Animal)->GetBrainComponent())
					UAIBlueprintHelperLibrary::GetAIController(Animal)->GetBrainComponent()->StopLogic("Dead");

			if (LiveAnimal)
			{
				if (PlayOnce)
				{
					APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
					FVector VecDif = Animal->GetActorLocation() - Planet->GetActorLocation();
					FRotator Rot = FRotationMatrix::MakeFromZ(VecDif).Rotator();
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LiveAnimal->VFX_Death, LiveAnimal->GetActorLocation(), Rot, FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);

					LiveAnimal->GetMesh()->PlayAnimation(LiveAnimal->Death, false);

					// Play death Niagara
					// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Animal->VFX_Death, LiveAnimal->GetActorLocation(), LiveAnimal->GetActorRotation(), FVector(1, 1, 1), true, true, ENCPoolMethod::AutoRelease);

					PlayOnce = false;
				}
			}

			Animal->IsDead = true;
			Animal->GetComponentByClass(UAttachableToPlanet::StaticClass())->Deactivate();
			Animal->GetMovementComponent()->SetActive(false);
		}
		if (Animal->age > Animal->AdultAge)
		{
		}
		if (Animal->age > Animal->DeathAge)
		{
			int RandomDeath = FMath::RandRange(1, DeathTime);

			if (RandomDeath == DeathTime)
			{
				Animal->Health = -1;
				//LiveAnimal->GetMesh()->PlayAnimation(LiveAnimal->Death, false);
				//Animal->);
			}
		}
	}

	// ...
}