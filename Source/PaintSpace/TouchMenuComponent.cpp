// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "TouchMenuComponent.h"


UTouchMenuComponent::UTouchMenuComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	PrevFrameID = 0;
	Opacity = 0.0f;
	ShowMenu = false;
	PalmFacingPlayer = false;
	HasLeftHand = false;
}


void UTouchMenuComponent::BeginPlay()
{
	Super::BeginPlay();

	// No menu item, shouldn't occur, and can't do anything with this instance
	if (!MenuItem)
	{
		Super::DestroyComponent();
	}
	MenuItem->AttachTo(this);
	//MenuItem->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//MenuItem->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MenuItem->SetMobility(EComponentMobility::Movable);
	DynamicMat = UMaterialInstanceDynamic::Create(MenuItem->GetMaterial(0), MenuItem);
	MenuItem->SetMaterial(0, DynamicMat);
}


void UTouchMenuComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckLeapFrame();
	DetermineVisibility();
	DetermineLocation();

	//float Recv;
	//DynamicMat->GetScalarParameterValue("Opacity", Recv);
	//FString dbgmsg = FString::SanitizeFloat(Recv);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);
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
				ShowMenu = false;
				PalmFacingPlayer = false;
			}
			PrevFrameID = latestFrame.id();
		}
	}
}


void UTouchMenuComponent::DetermineVisibility()
{
	if (!HasLeftHand)
	{
		Opacity = 0.0f;
	}
	else if (PalmFacingPlayer && Opacity < 1.0f)
	{
		Opacity += 0.02f;
	}
	else if (!PalmFacingPlayer && Opacity > 0.0f)
	{
		Opacity -= 0.02f;
	}
	DynamicMat->SetScalarParameterValue("Opacity", Opacity);
}


void UTouchMenuComponent::DetermineLocation()
{

}