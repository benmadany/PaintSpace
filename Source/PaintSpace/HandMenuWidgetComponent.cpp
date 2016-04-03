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
	PalmFacingPlayer = false;
	MenuShowing = false;
	HasLeftHand = false;

}


void UHandMenuWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!WidgetClass)
	{
		// no menu declared
	}

	HandMenuWidgetInstance = Cast<UHandMenuWidget>(Widget);
}

void UHandMenuWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckLeapFrame();
	ChangeMenuState();

/*	if (Widget->IsPlayingAnimation())
	{
		FString dbg = FString("test");
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, dbg);
	}*/
}

void UHandMenuWidgetComponent::CheckLeapFrame()
{
	bool LeftHandSeen = false;
	if (LeapController.isConnected())
	{
		const Frame latestFrame = LeapController.frame();
		if (latestFrame.id() != PrevFrameID)
		{
			for (Hand hand : latestFrame.hands())
			{

				if (hand.isLeft())
				{
					LeftHandSeen = true;
					HasLeftHand = true;
					if (hand.palmNormal().angleTo(Leap::Vector::down()) <= PI / 4.0f)
					{
						PalmFacingPlayer = true;
					}
					else
					{
						PalmFacingPlayer = false;
					}
				}
			}
			if (!LeftHandSeen)
			{
				HasLeftHand = false;
			}
			PrevFrameID = latestFrame.id();
		}
	}
}

void UHandMenuWidgetComponent::ChangeMenuState()
{
	if (PalmFacingPlayer)
	{
		if (!MenuShowing && !Widget->IsPlayingAnimation())
		{
			Widget->PlayAnimation(HandMenuWidgetInstance->ShowAnimationRef);
			MenuShowing = true;
		}
	}
	else
	{
		if (MenuShowing && !Widget->IsPlayingAnimation())
		{
			Widget->PlayAnimation(HandMenuWidgetInstance->HideAnimationRef);
			MenuShowing = false;
		}
	}
	if (!HasLeftHand)
	{
		MenuShowing = false;
		PalmFacingPlayer = false;
		Widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

/*void UHandMenuWidgetComponent::OnHMWComponentOverlapBegin(AActor * OtherActor, PrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void UHandMenuWidgetComponent::OnHMWComponentOverlapEnd(AActor * OtherActor, PrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{

}*/