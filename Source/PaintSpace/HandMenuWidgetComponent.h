// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Leap_NoPI.h"
#include "HandMenuWidgetComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PAINTSPACE_API UHandMenuWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
	
public:
	UHandMenuWidgetComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HMWidgetTemplate;

private:
	UUserWidget* HMWidgetInstance;
	Leap::Controller LeapController;
	int64_t PrevFrameID;
	
};
