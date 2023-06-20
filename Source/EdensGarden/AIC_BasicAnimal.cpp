// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_BasicAnimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
AAIC_BasicAnimal::AAIC_BasicAnimal(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/ProgrammerBasics/BT_BasicAnimal.BT_BasicAnimal'"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	behavior_treeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void AAIC_BasicAnimal::BeginPlay()
{
	Super::BeginPlay();


}
void AAIC_BasicAnimal::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	if (blackboard)
	{
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);

	}
	RunBehaviorTree(btree);
	behavior_treeComponent->StartTree(*btree);
}

void AAIC_BasicAnimal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

UBlackboardComponent* AAIC_BasicAnimal::GetBlackboard()
{
	return blackboard;
}