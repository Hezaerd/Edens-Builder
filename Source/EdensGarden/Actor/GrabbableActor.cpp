// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabbableActor.h"

#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"

#include "MotionControllerComponent.h"

#include "../Characters/Herbivore.h"
#include "../Characters/Carnivore.h"
#include "../Characters/Vegetation.h"
#include "../Player/HandsVR.h"
#include "../Basic/Planet.h"

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

// Sets default values
AGrabbableActor::AGrabbableActor()
{
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = Root;
	bIsHeld = false;

	// Can Tick
	PrimaryActorTick.bCanEverTick = true;
	//SOUNDS

	static ConstructorHelpers::FObjectFinder<USoundWave> propellerCue(TEXT("SoundWave'/Game/Assets/Sounds/Effects/A_Drop.A_Drop'"));

	propellerAudioCue = propellerCue.Object;
	propellerAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PropellerAudioComp"));

	propellerAudioComponent->bAutoActivate = false;
	propellerAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void AGrabbableActor::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AHandsVR>(UGameplayStatics::GetActorOfClass(GetWorld(), AHandsVR::StaticClass()));

	check(Player);

	StartLocation = this->GetActorLocation();
	StartRotation = this->GetActorRotation();

	if (propellerAudioCue->IsValidLowLevelFast()) {
		propellerAudioComponent->SetSound(propellerAudioCue);
	}
}

void AGrabbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHeld)
	{
		// Draw a raycast forwards from the controller
		FVector Start = CurrentMotionController->GetComponentLocation();
		FVector End = Start + (CurrentMotionController->GetForwardVector() * 1000);
		FHitResult HitResult;

		// If the raycast hits the planet, the player can drop the actor
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1))
		{
			if (HitResult.GetActor()->ActorHasTag("Planet2"))
			{
				// Yes the player can drop the actor
				Player->bCanDrop = true;

				// Draw the raycast in the world
				DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, -1, 0.5f);

				// Set the drop location on the normal of HitResult
				DropLocation = HitResult.ImpactPoint + (HitResult.ImpactNormal * 50);
			}
		}
		else
		{
			// No the player can't drop the actor
			Player->bCanDrop = false;
			DropLocation = FVector::Zero();
			// Draw the raycast in the world
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 0.5f);
		}
	}
}

void AGrabbableActor::Pickup_Implementation(USkeletalMeshComponent* MotionController)
{
	bIsHeld = true;

	StartRotation = GetActorRotation();
	CurrentMotionController = MotionController;
	Root->SetSimulatePhysics(false);
	Root->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform);
}

void AGrabbableActor::Drop_Implementation(USkeletalMeshComponent* MotionController)
{
	if ((MotionController == nullptr) || (CurrentMotionController == MotionController))
	{
		bIsHeld = false;

		Root->SetSimulatePhysics(bSimulatePhysics);

		CurrentMotionController = nullptr;

		Root->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		// Drop a new actor if the player is pointing the planet
		if (Player->bCanDrop)
		{
			/* Declare every actors here by using tags */
			// TODO: Create an independent class for every animals and plants instead of using tags
			// TODO: Find a way to spawn the actor without using the path
			if (this->ActorHasTag("Bunny"))
			{
				UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Herbivore/BP_Bunny.BP_Bunny")));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);

				FActorSpawnParameters SpawnParams;
				propellerAudioComponent->Play();
				GetWorld()->SpawnActor<AHerbivore>(GeneratedBP->GeneratedClass, DropLocation, this->GetActorRotation(), SpawnParams);
			}
			else if (this->ActorHasTag("Tree"))
			{
				UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Plant/Tree.Tree")));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);

				propellerAudioComponent->Play();
				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<AVegetation>(GeneratedBP->GeneratedClass, DropLocation, this->GetActorRotation(), SpawnParams);
			}
			else if (this->ActorHasTag("Deer"))
			{
				UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Herbivore/BP_Deer.BP_Deer")));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
				propellerAudioComponent->Play();
				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<AHerbivore>(GeneratedBP->GeneratedClass, DropLocation, this->GetActorRotation(), SpawnParams);
			}
			else if (this->ActorHasTag("Elephant"))
			{
				UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Herbivore/BP_Elephant.BP_Elephant")));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
				propellerAudioComponent->Play();
				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<AHerbivore>(GeneratedBP->GeneratedClass, DropLocation, this->GetActorRotation(), SpawnParams);
			}
			else if (this->ActorHasTag("Wolf"))
			{
				UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Carnivore/BP_Wolf.BP_Wolf")));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
				propellerAudioComponent->Play();
				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<ACarnivore>(GeneratedBP->GeneratedClass, DropLocation, this->GetActorRotation(), SpawnParams);
			}
			else if (this->ActorHasTag("Shark"))
			{
				UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Carnivore/BP_Shark.BP_Shark")));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
				propellerAudioComponent->Play();
				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<ACarnivore>(GeneratedBP->GeneratedClass, DropLocation, this->GetActorRotation(), SpawnParams);
			}
			else if (this->ActorHasTag("Kraken"))
			{
				UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/Carnivore/BP_Kraken.BP_Kraken")));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
				propellerAudioComponent->Play();
				FActorSpawnParameters SpawnParams;
				GetWorld()->SpawnActor<ACarnivore>(GeneratedBP->GeneratedClass, DropLocation, this->GetActorRotation(), SpawnParams);
			}

			//this->AttachToActor(Player, FAttachmentTransformRules::KeepWorldTransform);
		}

		// Snap back the actor to it's original position and rotation
		this->SetActorLocation(StartLocation);
		this->SetActorRotation(StartRotation);
	}
}