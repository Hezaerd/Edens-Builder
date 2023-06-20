// Fill out your copyright notice in the Description page of Project Settings.

#include "HerbivoreEatVegetation.h"
#include "RotateTargetWithPlanet.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../../Basic/Planet.h"
#include "../../Characters/Herbivore.h"
#include "../../Characters/Vegetation.h"
#include "AIController.h"
#include "Animation/AnimSequence.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

EBTNodeResult::Type UHerbivoreEatVegetation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	APlanet* planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	Vegetation = Cast<AVegetation>(owner_comp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("EatingVegetation"));
	Character = Cast<AHerbivore>(owner_comp.GetAIOwner()->GetCharacter());
	Character->GetMesh()->PlayAnimation(Character->Running, true);
	if (!Vegetation)
	{
		owner_comp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("CanEat", false);

		return EBTNodeResult::Succeeded;
	}
	FVector VecDif = Character->GetActorLocation() - planet->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromZ(VecDif).Rotator();
	UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Character->VFX_Angry, Character->GetActorLocation(), Rot, FVector(0.0001f, 0.0001f, 0.0001f), true, true, ENCPoolMethod::AutoRelease);

	if (Character->CurrentHunger < Character->MaxHunger && Vegetation->Health > 0)
	{
		IsDoneEating = false;
	}

	if (IsDoneEating)
	{
		owner_comp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("CanEat", false);

		return EBTNodeResult::Succeeded;
	}

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void UHerbivoreEatVegetation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!Vegetation)
	{
		Vegetation = Cast<AVegetation>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("EatingVegetation"));
		Character->GetMesh()->PlayAnimation(Character->Walking, true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	if (!Character)
	{
		Character = Cast<AHerbivore>(OwnerComp.GetAIOwner()->GetCharacter());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	if (Vegetation)
	{
		Character->CurrentHunger += Vegetation->NutritionalValue * DeltaSeconds;
		Vegetation->Health -= Vegetation->NutritionalValue * DeltaSeconds;

		if (Vegetation->Health < 0 || Character->CurrentHunger >= Character->MaxHunger)
		{
			IsDoneEating = true;
			Character->GetMesh()->PlayAnimation(Character->Walking, true);
			// TODO: STOP niagara effect
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool("CanEat", false);
			APlanet* planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}