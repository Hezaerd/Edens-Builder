// Fill out your copyright notice in the Description page of Project Settings.

#include "HerbivoreAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
AHerbivoreAI::AHerbivoreAI(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> GameObject(TEXT("BehaviorTree'/Game/Characters/Herbivore/AI/BT_Herbivore.BT_Herbivore'"));
	if (GameObject.Succeeded())
	{
		BTree = GameObject.Object;
	}
	BehaviorTreeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void AHerbivoreAI::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
	RunBehaviorTree(BTree);
	BehaviorTreeComponent->StartTree(*BTree);
}