// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "PaintBrushComponent.h"
#include "PaintMaterial.h"

using namespace Leap;

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
			const FVector ParentSpawnLocation = GetOwner()->GetActorLocation();
			//FVector(58.3f, 2.65f, 63.3f) offset...
			const FVector SpawnLocation = IndexFinger.tipPosition().toVector3<FVector>() + FVector(58.3f, 2.65f, 63.3f) + ParentSpawnLocation;
			// spawn rotation
			const FRotator ControlRotation = GetComponentRotation();
			FRotationMatrix RotationMatrix(ControlRotation); // not sure if necessary
			const FRotator SpawnRotation = ControlRotation;
			// spawn static mesh
			world->SpawnActor<APaintMaterial>(PaintMaterial, SpawnLocation, SpawnRotation);

			FString dbgmsg = FString((SpawnLocation.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);
		}
	}
}