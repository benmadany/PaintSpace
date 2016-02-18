// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "HandMenuWidgetComponent.h"
#include "HandMenuWidget.h"

using namespace Leap;


UHandMenuWidgetComponent::UHandMenuWidgetComponent()
	: Super()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	PrevFrameID = 0;
	ShowMenu = false;
}


void UHandMenuWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!WidgetClass)
	{
		// no menu declared
	}
}

void UHandMenuWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LeapController.isConnected())
	{
		const Frame latestFrame = LeapController.frame();
		if (latestFrame.id() != PrevFrameID)
		{
			ShowMenu = false;
			for (Hand hand : latestFrame.hands())
			{
				if (hand.isLeft()) 
				{
					ShowMenu = true;
				}
			}

			PrevFrameID = latestFrame.id();
		}

	}

	if (ShowMenu)
	{
		SetHiddenInGame(false, true);
		//FString dbgmsg = FString("Show Widget");
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);
	}
	if (!ShowMenu) 
	{
		SetHiddenInGame(true, true);
		//FString dbgmsg = FString("Hide Widget");
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);
	}
		
}