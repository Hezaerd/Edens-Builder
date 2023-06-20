// Fill out your copyright notice in the Description page of Project Settings.

#include "CarnivoreAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
ACarnivoreAI::ACarnivoreAI(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Characters/Carnivore/AI/BT_CarnivoreAI.BT_CarnivoreAI'"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	behavior_treeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void ACarnivoreAI::BeginPlay()
{
	Super::BeginPlay();
}
void ACarnivoreAI::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (blackboard)
	{
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}
	RunBehaviorTree(btree);
	behavior_treeComponent->StartTree(*btree);
}