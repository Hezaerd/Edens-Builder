// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAway.h"
#include "../../Characters/Herbivore.h"
#include "../../Basic/Planet.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"
#include "Components/SphereComponent.h"
EBTNodeResult::Type URunAway::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AHerbivore* Self = Cast<AHerbivore>(owner_comp.GetAIOwner()->GetCharacter());
	UBlackboardComponent* blackboard = owner_comp.GetAIOwner()->GetBlackboardComponent();
	
	if (Self->Chaser)
	{
		FVector Difference = Self->GetActorLocation() - Self->Chaser->GetActorLocation();
		Difference.Normalize();

		FVector RunVecAir = Self->GetActorLocation() + Difference * 5000;
		Self->GetMesh()->SetAnimation(Self->Walking);
		Self->GetMesh()->PlayAnimation(Self->Walking, true);
		APlanet* planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));

		FVector distanceVec = RunVecAir - planet->GetActorLocation();
		float distance = distanceVec.Length();
		distanceVec.Normalize();
		USphereComponent* a = Cast<USphereComponent>(planet->GetComponentByClass(USphereComponent::StaticClass()));

		FVector surfacepos = planet->GetActorLocation() + distanceVec * a->GetScaledSphereRadius();


		
		blackboard->SetValueAsVector("RunAwayVec", surfacepos);
		
	}
	blackboard->SetValueAsBool("TmpRunAway", false);
	return EBTNodeResult::Succeeded;
}