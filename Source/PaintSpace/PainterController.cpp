// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "PainterController.h"




APainterController::APainterController()
	: Super()
{
	
	PrevFrameID = -1;
	RightHandID = -1;
	LeftHandID = -1;
}

void APainterController::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine->HMDDevice.IsValid())
	{
		//IHeadMountedDisplay* HMD = GEngine->HMDDevice.Get();
		FString vrmsg = FString(TEXT("HMD Detected, optimizing controls."));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, vrmsg);
		LeapController.setPolicy(Leap::Controller::PolicyFlag::POLICY_OPTIMIZE_HMD);
	}
}

void APainterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LeapController.isConnected())
	{
		const Leap::Frame LatestFrame = LeapController.frame();
		if (LatestFrame.id() != PrevFrameID)
		{
			ProcessLeapFrame(LatestFrame);

			PrevFrameID = LatestFrame.id();
		}

	}
}


void APainterController::ProcessLeapFrame(Leap::Frame Frame)
{
	Leap::HandList HandList = Frame.hands();

	// two hands present, can make movement control gestures
	if (HandList.count() == 2)
	{
		// attempt to assign hands based on previously known hand IDs
		Leap::Hand RightHand = Frame.hand(RightHandID);
		Leap::Hand LeftHand = Frame.hand(LeftHandID);

		const Leap::Frame PrevFrame = LeapController.frame(1);

		// lacking either previous hand, retrieve new hands from frame
		if (!RightHand.isValid() || !LeftHand.isValid())
		{
			for (Leap::Hand h : HandList)
			{
				if (h.isRight())
				{
					RightHand = h;
					RightHandID = RightHand.id();
				}
				else if (h.isLeft())
				{
					LeftHand = h;
					LeftHandID = LeftHand.id();
				}
			}
		}
		
		// arbitrary grab strength float for now
		if (RightHand.grabStrength() > 0.8f && LeftHand.grabStrength() > 0.8f)
		{
			float RYChange = RightHand.translation(PrevFrame).y;
			float LYChange = LeftHand.translation(PrevFrame).y;

			if (RYChange > 0 && LYChange >0)
			{
				GetPawn()->SetActorLocation(GetPawn()->GetActorLocation() + FVector(((RYChange + LYChange) / 2), 0.0f, 0.0f));
				FString dbgmsg = FString::SanitizeFloat((RYChange + LYChange) / 2);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, dbgmsg);
			}
		}

	}
}