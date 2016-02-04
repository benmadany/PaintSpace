// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "HandMenuWidgetComponent.h"
#include "HandMenuWidget.h"




UHandMenuWidgetComponent::UHandMenuWidgetComponent()
	: Super()
{



}

void UHandMenuWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HMWidgetInstance)
	{
		if (!HMWidgetInstance)
		{
			HMWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HMWidgetTemplate);
		}
		if (!HMWidgetInstance->GetIsVisible())
		{
			HMWidgetInstance->AddToViewport();
		}
	}
}