// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "PainterController.h"
//#include "HandMenuWidget.h"




APainterController::APainterController()
	: Super()
{
	
}

void APainterController::BeginPlay()
{
	Super::BeginPlay();


	/*if (HandMenuWidgetTemplate)
	{
		if (!HandMenuWidgetInstance)
		{
			HandMenuWidgetInstance = CreateWidget<UHandMenuWidget>(this, HandMenuWidgetTemplate);
		}
		if (!HandMenuWidgetInstance->GetIsVisible())
		{
			//HandMenuWidgetInstance->AddToViewport();
		}
	}*/
}
