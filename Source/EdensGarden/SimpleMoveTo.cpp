// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include <EdensGarden/Characters/BasicAnimal.h>
EBTNodeResult::Type USimpleMoveTo::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	cont = owner_comp.GetAIOwner();
	AActor* Animal = cont->GetPawn();
	ACharacter* AnimalMovement = Cast<ACharacter>(Animal);
	Blackboard = owner_comp.GetBlackboardComponent();
	Blackboard->SetValueAsVector("TmpPoint", FVector(0,0,0));
	if (Blackboard->GetValueAsVector("TmpPoint") == FVector::Zero() && FVector::Dist(Animal->GetRootComponent()->GetComponentLocation(), Blackboard->GetValueAsVector(NextDestination.SelectedKeyName)) < MinRadius * Animal->GetActorScale().Length())
	{
		return EBTNodeResult::Succeeded;
	}
	else
	{
		
		UCharacterMovementComponent* movement = AnimalMovement->GetCharacterMovement();
		movement->Activate();
	}
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}


void USimpleMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	cont = OwnerComp.GetAIOwner();
	Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Animal = cont->GetPawn();
	Character = Cast<ACharacter>(Animal);

	if (!planet)
	{
		planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	}

	AController* myCharacterController = Character->GetController();
	AnimalAI = Cast<AAIC_BasicAnimal>(myCharacterController);
	if (ActiveCollision)
	{
		if (Blackboard->GetValueAsBool("TurnRight"))
		{
			RotateRight();
		}
		else if (Blackboard->GetValueAsBool("TurnLeft"))
		{
			RotateLeft();
		}
	}

	UCharacterMovementComponent* movement = Character->GetCharacterMovement();
	FVector BackBoardVec = Blackboard->GetValueAsVector(NextDestination.SelectedKeyName);
	FVector FinalVec = BackBoardVec - Animal->GetActorLocation();
	//DrawDebugPoint(GetWorld(), BackBoardVec, 30, FColor::Red);


	FRotator AnimalAngle(Animal->GetActorRotation());

	FinalVec.Normalize();
	FVector a;
	if (Blackboard->GetValueAsVector("TmpPoint") == FVector::Zero())
	{
		a = FinalVec;
	}
	else
	{
		//DrawDebugPoint(GetWorld(), Blackboard->GetValueAsVector("TmpPoint"), 30, FColor::Blue);
		FVector dist = Blackboard->GetValueAsVector("TmpPoint") - Character->GetActorLocation();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TMP POINT %f"), dist.Length()));
		a = Blackboard->GetValueAsVector("TmpPoint") - Character->GetActorLocation();
		a.Normalize();
	}
	if (Blackboard->GetValueAsVector("TmpPoint") != FVector::Zero() && FVector::Dist(Animal->GetRootComponent()->GetComponentLocation(), Blackboard->GetValueAsVector("TmpPoint")) < MinRadius * Animal->GetActorScale().Length())
	{
		Blackboard->SetValueAsVector("TmpPoint", FVector::Zero());
	}
	else if (Blackboard->GetValueAsVector("TmpPoint") == FVector::Zero() && FVector::Dist(Animal->GetRootComponent()->GetComponentLocation(), BackBoardVec) < MinRadius * Animal->GetActorScale().Length())
	{
		movement->Deactivate();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	FVector NormaliseVelocity = movement->Velocity;
	NormaliseVelocity.Normalize();
	FVector movementvector = Animal->GetActorLocation() + (NormaliseVelocity * 1000);
	FVector normalizeddistance = (Animal->GetActorLocation() - planet->GetActorLocation());
	//Check degrees with planet

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

//	DrawDebugLine(GetWorld(), Animal->GetActorLocation(), movementvector, FColor::Emerald, true, -1, 0, 10);

	normalizeddistance.Normalize();

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(Animal->GetActorLocation(), movementvector);

	FVector distanceVec = Animal->GetActorLocation() - planet->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromZ(distanceVec).Rotator();


	FVector Forward = -a;

	FVector WorldUp(Animal->GetActorLocation() - planet->GetActorLocation());
	//WorldUp.Normalize();
	FVector NewRight = Forward.Cross(WorldUp);
	//newRight.Normalize();
	FVector newUp = NewRight.Cross(Forward);

	FVector newForward = NewRight.Cross(WorldUp);


	//newUp.Normalize();
	FTransform transform = FTransform(newForward, NewRight, WorldUp, Animal->GetActorLocation());

	//TOP OF PLANET BUG FIX HERE
	FQuat rottohave(transform.Rotator());
	FQuat currentrot(Animal->GetActorRotation());

		
	FQuat newVal = FMath::Lerp(currentrot, rottohave, 0.05f);

	Animal->SetActorRotation(newVal);
	newForward.Normalize();


	ABasicAnimal* CurrentAnimal = Cast<ABasicAnimal>(Animal);
	if (IsRunning)
	{
		movement->Velocity = newForward * CurrentAnimal->RunSpeed;

	}
	else
	{
		movement->Velocity = newForward * CurrentAnimal->WalkSpeed;
	}
}

void USimpleMoveTo::RotateLeft()
{
	//MoveSpeed = 100;
	AActor* otherActor = Cast<AActor>(Blackboard->GetValueAsObject("CollisionObject"));
	FVector TouchPointAir = otherActor->GetActorLocation() - Character->GetActorRightVector() * FMath::Abs(otherActor->GetActorScale().SquaredLength() * 32.f / otherActor->GetActorScale().Length());
	FVector distanceVec =  TouchPointAir - planet->GetActorLocation();
	float distance = distanceVec.Length();
	distanceVec.Normalize();
	USphereComponent* a = Cast<USphereComponent>(planet->GetComponentByClass(USphereComponent::StaticClass()));

	FVector TouchPoint = planet->GetActorLocation() + distanceVec * a->GetScaledSphereRadius();
	TouchPoint += Character->GetActorScale() / 2;
	Blackboard->SetValueAsVector("TmpPoint", TouchPoint);

	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Name %s"), *TmpEndPoint.ToString()));
}
void USimpleMoveTo::RotateRight()
{
	AActor* otherActor = Cast<AActor>(Blackboard->GetValueAsObject("CollisionObject"));
	float Scale = FMath::Abs(otherActor->GetActorScale().SquaredLength() * 32.f / otherActor->GetActorScale().Length());
	FVector TouchPointAir = otherActor->GetActorLocation() + Character->GetActorRightVector() * Scale + Character->GetActorForwardVector() * Scale;
	FVector distanceVec = TouchPointAir - planet->GetActorLocation();
	float distance = distanceVec.Length();
	distanceVec.Normalize();
	USphereComponent* a = Cast<USphereComponent>(planet->GetComponentByClass(USphereComponent::StaticClass()));

	FVector TouchPoint = planet->GetActorLocation() + distanceVec * a->GetScaledSphereRadius();
	TouchPoint += Character->GetActorScale() / 2;
	Blackboard->SetValueAsVector("TmpPoint", TouchPoint);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ENDPOINT %s"), *TmpEndPoint.ToString()));
}