// Fill out your copyright notice in the Description page of Project Settings.

#include "MateCarnivores.h"
#include "AIController.h"
#include "../../Characters/Carnivore.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UMateCarnivores::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Ai = owner_comp.GetAIOwner();
	ACarnivore* Animal = Cast<ACarnivore>(Ai->GetCharacter());
	Blackboard = owner_comp.GetBlackboardComponent();
	ACarnivore* OtherCarnivore = Animal->otherCarnivore;
	Animal->ReproductionPoints = 0;
	if (!OtherCarnivore->IsMating)
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
		FVector Location = Animal->GetActorLocation();
		FRotator Rotation = Animal->GetActorRotation();

		ACarnivore* NewCarnivore = GetWorld()->SpawnActor<ACarnivore>(GeneratedBP->GeneratedClass, Location, Rotation, SpawnParams);

		NewCarnivore->Generation += Animal->Generation;
	}
	Blackboard->SetValueAsBool("CanMate", false);
	Animal->otherCarnivore = nullptr;
	Animal->GetMesh()->PlayAnimation(Animal->Walking, true);
	Blackboard->SetValueAsObject("OtherCarnivore", nullptr);
	return EBTNodeResult::Succeeded;
}