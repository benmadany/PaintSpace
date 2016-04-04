// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "TouchMenuComponent.h"


UTouchMenuComponent::UTouchMenuComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	PrevFrameID = 0;
}


void UTouchMenuComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UTouchMenuComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	CheckLeapFrame();
}


void UTouchMenuComponent::CheckLeapFrame()
{
	bool LeftHandSeen = false;
	if (LeapController.isConnected())
	{
		const Leap::Frame latestFrame = LeapController.frame();
		if (latestFrame.id() != PrevFrameID)
		{
			for (Leap::Hand hand : latestFrame.hands())
			{

				if (hand.isLeft())
				{
					LeftHandSeen = true;
					if (hand.palmNormal().angleTo(Leap::Vector::down()) <= PI / 4.0f)
					{
					}
					else
					{
					}
				}
			}
			if (!LeftHandSeen)
			{
			}
			PrevFrameID = latestFrame.id();
		}
	}
}

