// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Leap_NoPI.h"
#include "TouchMenuComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAINTSPACE_API UTouchMenuComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UTouchMenuComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Tree")
	UTouchMenuComponent* ParentTMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Tree")
	TArray<UTouchMenuComponent*> TMChildren;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Item")
	UStaticMeshComponent* MenuItem;

private:
	void CheckLeapFrame();
	void DetermineVisibility();
	void DetermineLocation();

	Leap::Controller LeapController;
	int64_t PrevFrameID;
	UMaterialInstanceDynamic* DynamicMat;
	
	float Opacity;
	bool PalmFacingPlayer;
	bool ShowMenu;
	bool HasLeftHand;
};
