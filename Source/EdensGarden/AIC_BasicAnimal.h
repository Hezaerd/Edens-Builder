// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_BasicAnimal.generated.h"

/**
 * 
 */
UCLASS()
class EDENSGARDEN_API AAIC_BasicAnimal : public AAIController
{
	GENERATED_BODY()
public:

	AAIC_BasicAnimal(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;
	void Tick(float DeltaSeconds) override;
	UBlackboardComponent* GetBlackboard();

	bool RotateLeft;
	bool RotateRight;


	AActor* HitActor;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* behavior_treeComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* btree;


	class UBlackboardComponent* blackboard;



	class UAISenseConfig_Sight* sight_config;



//	UFUNCTION()

};
