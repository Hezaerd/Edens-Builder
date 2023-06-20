// Fill out your copyright notice in the Description page of Project Settings.

#include "MateHerbivores.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../../Statistics/StatsTracker.h"
#include "AIController.h"
#include "../../Characters/Herbivore.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMateHerbivores::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Ai = owner_comp.GetAIOwner();
	AHerbivore* Animal = Cast<AHerbivore>(Ai->GetCharacter());
	Blackboard = owner_comp.GetBlackboardComponent();

	
	AHerbivore* OtherHerbivore = Animal->otherHerbivore;
	Animal->ReproductionPoints = 0;
	if (!OtherHerbivore->IsMating && Animal->Chaser == nullptr)
	{
		UClass* Tmpclass = Animal->GetClass();

		FString PathName = Tmpclass->GetPathName();
		PathName.RemoveAt(PathName.Len() - 2, 2);
		UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, *PathName));

		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		if (!SpawnActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Animal->IsMating = true;
		AStatsTracker* Tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));
		FVector Location = Animal->GetActorLocation();
		FRotator Rotation = Animal->GetActorRotation();
		AHerbivore* newHerbivore = GetWorld()->SpawnActor<AHerbivore>(GeneratedBP->GeneratedClass, Location, Rotation, SpawnParams);
		if (Tracker)
			Tracker->AddEntity(newHerbivore);

		newHerbivore->Generation += Animal->Generation;
	}
	Blackboard->SetValueAsBool("CanMate", false);
	Animal->otherHerbivore = nullptr;
	Blackboard->SetValueAsObject("OtherHerbivore", nullptr);
	Animal->GetMesh()->PlayAnimation(Animal->Walking, true);
	return EBTNodeResult::Succeeded;
}