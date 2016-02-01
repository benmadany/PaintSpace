// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "PaintBrushComponent.h"
#include "PaintMaterial.h"

using namespace Leap;

// This class should be attached to some scene component in order to provide paint functionality based on sockets.

// Sets default values for this component's properties
UPaintBrushComponent::UPaintBrushComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	PrevFrameID = 0;
	
}


// Called when the game starts
void UPaintBrushComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UPaintBrushComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ensure we are connected to the device
	if (LeapController.isConnected())
	{
		// get latest frame
		const Frame latestFrame = LeapController.frame();
		// check to see if it is different than the previous
		if (latestFrame.id() != PrevFrameID)
		{
			CheckHand(latestFrame);

			PrevFrameID = latestFrame.id();
		}

	}

}

void UPaintBrushComponent::CheckHand(Frame frame)
{
	// look for the right hand
	for (Hand hand : frame.hands()) {
		if (hand.isRight())
		{
			// need to include checking for gestures
			TryPainting(hand);
		}
	}
}

void UPaintBrushComponent::TryPainting(Hand hand)
{
	if (PaintMaterial != NULL)
	{
		UWorld* const world = GetWorld();
		if (world != NULL) {

			Finger IndexFinger;
			for (Finger finger : hand.fingers())
			{
				if (finger.type() == Finger::Type::TYPE_INDEX)
				{
					IndexFinger = finger;
					break;
				}
			}
			// spawn location
				//Vector TipPositionLeap = IndexFinger.tipPosition();
				// convert from LeapMotion vector to Unreal Engine vector
				//FVector TipPositionUnreal = FVector(TipPositionLeap.y, -TipPositionLeap.x, -TipPositionLeap.z);
			// Use skeletal mesh socket to get accurate finger position
			FName IndexFingerSocket;
			if (AttachParent->GetAllSocketNames().IsValidIndex(0))
				IndexFingerSocket = FName(AttachParent->GetAllSocketNames()[0]);
			FVector FingerOffset = FVector(0, 0.0f, 0.0f); // not needed
			const FVector SpawnLocation = (AttachParent->GetSocketLocation(IndexFingerSocket)) + FingerOffset;

			// spawn rotation
			const FRotator SpawnRotation = SpawnLocation.Rotation();

			// spawn static mesh
			world->SpawnActor<APaintMaterial>(PaintMaterial, SpawnLocation, SpawnRotation);

			FString dbgmsg = FString((SpawnLocation.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);
		}
	}
}