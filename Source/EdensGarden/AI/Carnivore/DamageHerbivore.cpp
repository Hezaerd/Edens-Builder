// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageHerbivore.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../../Characters/Carnivore.h"
#include "../../Characters/Herbivore.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "AIController.h"
#include "../../Statistics/StatsTracker.h"
#include "Animation/AnimSequence.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

EBTNodeResult::Type UDamageHerbivore::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Tracker = Cast<AStatsTracker>(UGameplayStatics::GetActorOfClass(GetWorld(), AStatsTracker::StaticClass()));
	if (IsCarnivore)
	{
		ACarnivore* Self = Cast<ACarnivore>(owner_comp.GetAIOwner()->GetCharacter());
		ACarnivore* Prey = Self->otherEatableCarnivore;
		if (PlayOnce)
		{
			PlayOnce = false;
			Self->GetMesh()->PlayAnimation(Self->Running, true);
			Prey->GetMesh()->PlayAnimation(Prey->Running, true);
		}
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Self->VFX_Angry, Self->GetActorLocation(), Self->GetActorRotation(), FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);
		if (!Prey)
		{
			PlayOnce = true;
			Self->GetMesh()->PlayAnimation(Self->Walking, true);
			return EBTNodeResult::Succeeded;
		}
		Prey->Health -= Self->Damage * Self->GetWorld()->GetDeltaSeconds();
		Tracker->CarnivoreAttacked = true;
		return EBTNodeResult::Succeeded;
	}
	ACarnivore* Self = Cast<ACarnivore>(owner_comp.GetAIOwner()->GetCharacter());
	if (PlayOnce)
	{
		PlayOnce = false;
		Self->GetMesh()->PlayAnimation(Self->Running, true);
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Self->VFX_Angry, Self->GetActorLocation(), Self->GetActorRotation(), FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);
	AHerbivore* Prey = Self->otherHerbivore;
	if (!Prey)
	{
		PlayOnce = true;
		Self->GetMesh()->PlayAnimation(Self->Walking, true);
		return EBTNodeResult::Succeeded;
	}
	Prey->Health -= Self->Damage * Self->GetWorld()->GetDeltaSeconds();

	return EBTNodeResult::Succeeded;
}