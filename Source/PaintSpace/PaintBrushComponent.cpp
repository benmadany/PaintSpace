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

	if (LeapController.isConnected())
	{
		const Frame latestFrame = LeapController.frame();
		FString dbgmsg(latestFrame.toString().c_str());

		if (PaintMaterial != NULL)
		{
			UWorld* const world = GetWorld();
			if (world != NULL) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, dbgmsg);
				const FRotator SpawnRotation = AttachParent->GetComponentRotation();
				const FVector SpawnLocation = AttachParent->GetComponentLocation();
				world->SpawnActor<APaintMaterial>(PaintMaterial, SpawnLocation, SpawnRotation);
			}
		}
		
	}

}

