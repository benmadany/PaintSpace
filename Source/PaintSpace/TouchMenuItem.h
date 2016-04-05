// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TouchMenuItem.generated.h"

UCLASS()
class PAINTSPACE_API ATouchMenuItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ATouchMenuItem();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	void UpdateOpacity(float InOpacity);

	UStaticMeshComponent* GetRootSMC();

	FVector GetLocationOffset();

	void SetLocationOffset(FVector NewOffset);

private:


	UMaterialInstanceDynamic* DynamicMat;
	FVector Offset;

	float Opacity;
};
