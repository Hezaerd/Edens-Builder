// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionDetection.h"
#include "Components/ActorComponent.h"
#include "BasicAnimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../SimpleMoveTo.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
// Sets default values for this component's properties
UCollisionDetection::UCollisionDetection()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Character = Cast<ABasicAnimal>(GetOwner());
	
}


// Called when the game starts
void UCollisionDetection::BeginPlay()
{
	Super::BeginPlay();
	planet = Cast<APlanet>(UGameplayStatics::GetActorOfClass(GetWorld(), APlanet::StaticClass()));
	//

}


// Called every frame
void UCollisionDetection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Character = Cast<ABasicAnimal>(GetOwner());
	AController* myEnemyController = Character->GetController();
	AAIController* myAIEnemyController;
	myAIEnemyController = Cast<AAIController>(myEnemyController);
	Blackboard = myAIEnemyController->GetBlackboardComponent();


	currentlocation = Character->GetActorLocation() + Character->GetActorUpVector() * 200;
	UpdateVectors();
	VectorCollision();
}

void UCollisionDetection::UpdateVectors()
{
	ForwardVector = currentlocation + Character->GetActorForwardVector() * planet->GetActorScale().Length() * VectorScale;
	

	RightSideVector = currentlocation + (Character->GetActorForwardVector() + Character->GetActorRightVector()) * planet->GetActorScale().Length() * VectorScale /2;


	//DrawDebugLine(GetWorld(), currentlocation, RightSideVector, FColor::Red, false, -1.0f, 0U, 50);

	LeftSideVector = currentlocation + (Character->GetActorForwardVector() - Character->GetActorRightVector()) * planet->GetActorScale().Length() * VectorScale /2;

	//DrawDebugLine(GetWorld(), currentlocation, LeftSideVector, FColor::Red, false, -1.0f, 0U, 50);
}

void UCollisionDetection::VectorCollision()
{
	FHitResult OutHitForward;
	FHitResult OutHitRight;
	FHitResult OutHitLeft;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Character);
	//DrawDebugLine(GetWorld(), currentlocation, ForwardVector, FColor::Black, false, -1.0f, 0U, 50);
	AController* myCharacterController = Character->GetController();
	AAIC_BasicAnimal* AnimalAI = Cast<AAIC_BasicAnimal>(myCharacterController);
	do
	{
		if (!AnimalAI)
		{
			break;
		}
		Blackboard->SetValueAsBool("TurnRight", false);
		Blackboard->SetValueAsBool("TurnLeft", false);
		if (GetWorld()->LineTraceSingleByChannel(OutHitForward, currentlocation, ForwardVector, ECC_MAX, CollisionParams))
		{
			if (OutHitForward.GetActor()->ActorHasTag("OBSTACLE"))
			{
				if (GetWorld()->LineTraceSingleByChannel(OutHitRight, currentlocation, LeftSideVector, ECC_MAX, CollisionParams))
				{
					if (OutHitRight.GetActor()->ActorHasTag("OBSTACLE"))
					{

						Blackboard->SetValueAsBool("TurnRight", true);
						Blackboard->SetValueAsObject("CollisionObject", OutHitForward.GetActor());
						break;
					}

				}
				else if(GetWorld()->LineTraceSingleByChannel(OutHitLeft, currentlocation, LeftSideVector, ECC_MAX, CollisionParams))
				{
					if (OutHitLeft.GetActor()->ActorHasTag("OBSTACLE"))
					{
						Blackboard->SetValueAsBool("TurnLeft", true);
						Blackboard->SetValueAsObject("CollisionObject", OutHitForward.GetActor());
						break;
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ROTATE RIGHT"));
					}

				}
				else
				{ 

					Blackboard->SetValueAsBool("TurnRight", true);
					Blackboard->SetValueAsObject("CollisionObject", OutHitForward.GetActor());
					break;
				

				}
			}

		}
	} while (false);


}