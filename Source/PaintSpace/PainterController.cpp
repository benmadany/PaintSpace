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

	Character = GetPawn();
}

void APainterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LeapController.isConnected())
	{
		const Leap::Frame LatestFrame = LeapController.frame();
		if (LatestFrame.id() != PrevFrameID)
		{
			ProcessLeapFrame(LatestFrame, DeltaSeconds);

			PrevFrameID = LatestFrame.id();
		}

	}
}


void APainterController::ProcessLeapFrame(Leap::Frame Frame, float DeltaSeconds)
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
			Leap::Vector RChange = RightHand.translation(PrevFrame);
			Leap::Vector LChange = LeftHand.translation(PrevFrame);

			// forward
			if (RChange.y > 0 && LChange.y > 0)
			{
				Character->SetActorLocation(Character->GetActorLocation() + GetControlRotation().RotateVector(FVector((RChange.y + LChange.y) / -2, 0.0f, 0.0f)));

				//FString dbgmsg = FString::SanitizeFloat((RChange.y + LChange.y) / 2);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, dbgmsg);
			}

			// backward
			if (RChange.y < 0 && LChange.y < 0)
			{
				Character->SetActorLocation(Character->GetActorLocation() + GetControlRotation().RotateVector(FVector((RChange.y + LChange.y) / -2, 0.0f, 0.0f)));
			}

			// left
			if (RChange.x > 0 && LChange.x > 0)
			{
				Character->SetActorLocation(Character->GetActorLocation() + GetControlRotation().RotateVector(FVector(0.0f, (RChange.x + LChange.x) / 2, 0.0f)));
			}

			// right
			if (RChange.x < 0 && LChange.x < 0)
			{
				Character->SetActorLocation(Character->GetActorLocation() + GetControlRotation().RotateVector(FVector(0.0f, (RChange.x + LChange.x) / 2, 0.0f)));
			}

			// down
			if (RChange.z > 0 && LChange.z > 0)
			{
				Character->SetActorLocation(Character->GetActorLocation() + GetControlRotation().RotateVector(FVector(0.0f, 0.0f, (RChange.z + LChange.z) / 2)));
			}

			// up
			if (RChange.z < 0 && LChange.z < 0)
			{
				Character->SetActorLocation(Character->GetActorLocation() + GetControlRotation().RotateVector(FVector(0.0f, 0.0f, (RChange.z + LChange.z) / 2)));
			}

			// rotation left
			if (RChange.y > 0 && LChange.y < 0)
			{
				FRotator NewRotation = GetControlRotation() + FRotator(0.0f, (RChange.y + -LChange.y) / 4, 0.0f);
				SetControlRotation(NewRotation);
			}

			// rotation right
			if (RChange.y < 0 && LChange.y > 0)
			{
				FRotator NewRotation = GetControlRotation() - FRotator(0.0f, (-RChange.y + LChange.y) / 4, 0.0f);
				SetControlRotation(NewRotation);
			}
		}

	}
}