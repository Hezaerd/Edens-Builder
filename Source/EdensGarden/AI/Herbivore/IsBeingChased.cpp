// Fill out your copyright notice in the Description page of Project Settings.


#include "IsBeingChased.h"
#include "AIController.h"
#include "../../Characters/Herbivore.h"
bool UIsBeingChased::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AHerbivore* Self = Cast<AHerbivore>(OwnerComp.GetAIOwner()->GetCharacter());

	if (Self->IsBeingChased)
	{
		return true;
	}
	return false;
}