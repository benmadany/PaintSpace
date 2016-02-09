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
		/*if (!HMWidgetInstance->GetIsVisible())
		{
			HMWidgetInstance->AddToViewport();
		}*/
	}

	InitWidget();
}

void UHandMenuWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool ShowMenu = false;

	if (LeapController.isConnected())
	{
		const Frame latestFrame = LeapController.frame();
		if (latestFrame.id() != PrevFrameID)
		{
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

	/*if (ShowMenu)
		HMWidgetInstance->AddToViewport();
	if (!ShowMenu)
		HMWidgetInstance->RemoveFromViewport();
	if (ShowMenu)
	{
		Widget->AddToViewport();
		FString dbgmsg = FString("Show Widget");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);
	}
	if (!ShowMenu) 
	{
		Widget->RemoveFromViewport();
		RemoveWidgetFromScreen();
		FString dbgmsg = FString("Hide Widget");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);
	}*/
		
}