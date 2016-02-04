// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "Runtime/UMG/Public/UMG.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HMWidgetTemplate;

private:
	UUserWidget* HMWidgetInstance;
	
};
