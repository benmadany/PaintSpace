// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HandMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PAINTSPACE_API UHandMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temp")
	FString TempWidgetName;
	
};
