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

	//IndexFingerSocket = FName(*FString("rt_index_endSocket"));
}


// Called when the game starts
void UPaintBrushComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PaintMaterial)
	{
		UWorld* const world = GetWorld();
		if (world)
		{
			// spawn instanced static mesh actor, better performance than individual actors
			PaintMaterialInstance = world->SpawnActor<APaintMaterial>(PaintMaterial);
		}
	}

	if (GEngine->HMDDevice.IsValid())
	{
		//IHeadMountedDisplay* HMD = GEngine->HMDDevice.Get();
		FString vrmsg = FString(TEXT("HMD Detected, optimizing controls."));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, vrmsg);
		LeapController.setPolicy(Controller::PolicyFlag::POLICY_OPTIMIZE_HMD);
	}

	//IndexFingerSocket = AttachSocketName;
}


// Called every frame
void UPaintBrushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LeapController.isConnected())
	{
		const Frame latestFrame = LeapController.frame();
		if (latestFrame.id() != PrevFrameID)
		{
			CheckHand(latestFrame);

			PrevFrameID = latestFrame.id();
		}

	}

}


void UPaintBrushComponent::CheckHand(Frame frame)
{
	for (Hand hand : frame.hands())
	{
		if (hand.isRight())
		{
			// need to include checking for gestures
			TryPainting(hand);
		}
	}
}


void UPaintBrushComponent::TryPainting(Hand hand)
{

	Finger IndexFinger;
	for (Finger finger : hand.fingers())
	{
		if (finger.type() == Finger::Type::TYPE_INDEX)
		{
			IndexFinger = finger;
			break;
		}
	}

	// Use skeletal mesh socket to get accurate finger position
	//const FVector SpawnLocation = (AttachParent->GetSocketLocation(IndexFingerSocket));
	const FVector SpawnLocation = GetComponentLocation();

	// spawn rotation
	const FRotator SpawnRotation = SpawnLocation.Rotation();

	// spawn static mesh
	PaintMaterialInstance->VisibleComponent->AddInstance(FTransform(SpawnRotation, SpawnLocation, FVector(0.01, 0.01f, 0.05f)));

	FString dbgmsg = FString((SpawnLocation.ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);

}