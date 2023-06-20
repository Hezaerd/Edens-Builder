// Fill out your copyright notice in the Description page of Project Settings.

#include "RotateTargetWithPlanet.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Basic/Planet.h"
URotateTargetWithPlanet::URotateTargetWithPlanet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	StartRotation = FRotator(0, 0, 0);
}

void URotateTargetWithPlanet::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	StartRotation = Planet->GetActorRotation();
}

void URotateTargetWithPlanet::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Blackboard = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	FVector MoveToVec = Blackboard->GetValueAsVector(NextDestination.SelectedKeyName);
	APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));

	FRotator PlanetRotator = Planet->GetActorRotation();
	PlanetRotator -= StartRotation;
	FVector EndVec = PlanetRotator.RotateVector(MoveToVec - Planet->GetActorLocation());
	EndVec += Planet->GetActorLocation();
	Blackboard->SetValueAsVector("DestinationWithRotation", EndVec);
}

void URotateTargetWithPlanet::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlanet* Planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	StartRotation = Planet->GetActorRotation();
}