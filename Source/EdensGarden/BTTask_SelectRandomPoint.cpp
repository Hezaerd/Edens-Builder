// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectRandomPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Components/SphereComponent.h"
#include "Characters/BasicAnimal.h"

#include "AIController.h"
EBTNodeResult::Type UBTTask_SelectRandomPoint::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	if (!planet)
	{
		planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));

	}
	ABasicAnimal* Character = Cast<ABasicAnimal>(owner_comp.GetAIOwner()->GetCharacter());
	Character->IsWalking = true;
	
	UBlackboardComponent* Blackboard = owner_comp.GetBlackboardComponent();
	FRandomStream rand;
	rand.GenerateNewSeed();

	FVector RandomPoint = rand.GetUnitVector();

	USphereComponent* a = Cast<USphereComponent>(planet->GetComponentByClass(USphereComponent::StaticClass()));

	RandomPoint *= a->GetScaledSphereRadius();
	RandomPoint += planet->GetActorLocation();

	//DrawDebugPoint(GetWorld(), RandomPoint, 30, FColor(255, 0, 0));

	Blackboard->SetValueAsVector(NextDestination.SelectedKeyName, RandomPoint);
	PlayOnce = true;
	return EBTNodeResult::Succeeded;
}