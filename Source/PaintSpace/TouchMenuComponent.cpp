// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "TouchMenuComponent.h"


UTouchMenuComponent::UTouchMenuComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	PrevFrameID = 0;
	RootOpacity = 0.0f;
	ChildOpacity = 0.0f;
	Enabled = false;
	ShowMenu = false;
	PalmFacingPlayer = false;
	HasLeftHand = false;
	MenuOpen = false;
}


void UTouchMenuComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MenuItemTemplate)
	{
		UWorld* const world = GetWorld();
		if (world)
		{
			MenuItem = world->SpawnActor<ATouchMenuItem>(MenuItemTemplate);
		}
	}
	if (!MenuItem)
	{
		// No menu item, and can't do anything with this instance
		Super::DestroyComponent();
	}
}


void UTouchMenuComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckLeapFrame();
	DetermineVisibility();
	DetermineLocation();

	if (RootOpacity <= 0.0f)
	{
		CollapseMenu();
	}
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
						Enabled = true;
					}
					else
					{
						Enabled = false;
						PalmFacingPlayer = false;
					}
				}
			}
			if (!LeftHandSeen)
			{
				Enabled = false;
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
		RootOpacity = 0.0f;
		ChildOpacity = 0.0f;
	}
	else if (PalmFacingPlayer && RootOpacity < 1.0f)
	{
		RootOpacity += 0.02f;
		ChildOpacity += 0.02f;
	}
	else if (!PalmFacingPlayer && RootOpacity > 0.0f)
	{
		RootOpacity -= 0.02f;
		ChildOpacity -= 0.02f;
	}
	if (MenuOpen && PalmFacingPlayer)
	{
		RootOpacity = 0.4f;
	}
	MenuItem->UpdateOpacity(RootOpacity);

	if (!MenuOpen)
	{
		ChildOpacity = 0.0f;
	}
	for (ATouchMenuItem* ChildInstance : Children)
	{
		ChildInstance->UpdateOpacity(ChildOpacity);
	}
}


void UTouchMenuComponent::DetermineLocation()
{
	MenuItem->SetActorLocation(GetComponentLocation());
	MenuItem->SetActorRotation(GetComponentRotation());

	for (ATouchMenuItem* ChildInstance : Children)
	{
		ChildInstance->SetActorLocation(GetComponentLocation() + ChildInstance->GetLocationOffset());
		ChildInstance->SetActorRotation(GetComponentRotation());
	}
	// TODO: function for press/release animation
}


void UTouchMenuComponent::ExpandMenu()
{
	if (ChildTemplates.Num() > 0 && !MenuOpen)
	{
		for (TSubclassOf<ATouchMenuItem> Child : ChildTemplates)
		{
			ATouchMenuItem* ChildInstance = GetWorld()->SpawnActor<ATouchMenuItem>(Child);
			FVector ChildOffset = FVector(0, 5, -5 + (10 * Children.Num()));
			ChildInstance->SetLocationOffset(GetWorld()->GetFirstPlayerController()->GetControlRotation().RotateVector(ChildOffset));
			Children.Add(ChildInstance);
		}

		MenuOpen = true;
	}
}


void UTouchMenuComponent::CollapseMenu()
{
	for (ATouchMenuItem* Child : Children)
	{
		Child->Destroy();
	}
	Children.Empty();
	MenuOpen = false;
}


void UTouchMenuComponent::BPExpandMenu()
{
	ExpandMenu();
}