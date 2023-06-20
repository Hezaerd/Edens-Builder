// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Engine/DecalActor.h"
#include "Materials/Material.h"
#include "MissionScoreboard.generated.h"

UCLASS()
class EDENSGARDEN_API AMissionScoreboard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMissionScoreboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(editAnywhere)
		UStaticMeshComponent* Meshcomp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDecalComponent* DecalTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDecalComponent* DecalTick2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDecalComponent* DecalTick3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* ImageUnchecked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* ImageChecked;

	UMaterial* TickMat;
	UMaterial* NoTickMat;

	TArray<class UDecalComponent*> Decals;

	UMaterialInstance* MatInstance;

	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return nullptr;

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), nullptr, *Path.ToString()));
	}

	static FORCEINLINE UMaterial* AMissionScoreboard::LoadMaterialFromPath(const FName& Path)
	{
		if (Path == NAME_None) return nullptr;

		return LoadObjFromPath<UMaterial>(Path);
	}

	void FinishedMission(int numb);
};
