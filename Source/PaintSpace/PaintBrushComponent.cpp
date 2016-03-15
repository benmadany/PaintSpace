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
	ProceduralSectionIndex = 0;

	//IndexFingerSocket = FName(*FString("rt_index_endSocket"));
	ObjExporter ObjExp = ObjExporter();
	ObjExporterInstance = &ObjExp;

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
			LODModel = &PaintMaterialInstance->MeshComponent->StaticMesh->RenderData->LODResources[0];
			VertexBuffer = &LODModel->PositionVertexBuffer;
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

void UPaintBrushComponent::ClearAllStrokes()
{
	PaintMaterialInstance->MeshComponent->ClearInstances();
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
		else if (hand.isLeft())
		{
			int HandID = hand.id();
			if (hand.grabStrength() == 1.0 && LeapController.frame(1).hand(HandID).grabStrength() != 1.0) // temporary, not very accurate
				ExportObj();
		}
	}
}


void UPaintBrushComponent::TryPainting(Hand hand)
{

	Finger IndexFinger;
	for (Finger finger : hand.fingers())
	{
		if (finger.type() == Finger::Type::TYPE_THUMB)
		{
			if (finger.isExtended()) // too harsh right now
				return;
		}
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
	FVector ScaleVector = FVector(0.01f, 0.01f, 0.05f);
	PaintMaterialInstance->MeshComponent->AddInstance(FTransform(SpawnRotation, SpawnLocation, ScaleVector));

	int32 instances = PaintMaterialInstance->MeshComponent->PerInstanceSMData.Num();
	/*if (instances > 1)
	{
		GenerateProceduralMesh(PaintMaterialInstance->MeshComponent->PerInstanceSMData[instances - 2], PaintMaterialInstance->MeshComponent->PerInstanceSMData[instances - 1]);
	}*/
	//FString dbgmsg = FString(PaintMaterialInstance->MeshComponent->PerInstanceSMData.Last().Transform.ToString());

	//FString dbgmsg = FString((SpawnLocation.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);

}

void UPaintBrushComponent::GenerateProceduralMesh(FInstancedStaticMeshInstanceData PrevMesh, FInstancedStaticMeshInstanceData CurrentMesh)
{
	FVector ScaleVector = FVector(0.01f, 0.01f, 0.05f);

	FTransform PrevTransform = FTransform(PrevMesh.Transform);
	FTransform CurrentTransform = FTransform(CurrentMesh.Transform);
	bool forward = true;
	if (PrevTransform.GetTranslation().Z > CurrentTransform.GetTranslation().Z)
	{
		forward = false;
	}

	TArray<FVector> ProceduralVertices;
	TArray<int32> ProceduralTriangles;
	TArray<FColor> ProceduralColors;
	TArray<FVector> SecondPartition;

	for (uint32 i = 0; i < VertexBuffer->GetNumVertices() / 2; i += 4)
	{
		if (forward)
		{
			ProceduralVertices.Add((VertexBuffer->VertexPosition(i + 2) + PrevTransform.GetTranslation()) * ScaleVector);
			ProceduralVertices.Add((VertexBuffer->VertexPosition(i + 3) + PrevTransform.GetTranslation()) * ScaleVector);
			SecondPartition.Add((VertexBuffer->VertexPosition(i) + CurrentTransform.GetTranslation()) * ScaleVector);
			SecondPartition.Add((VertexBuffer->VertexPosition(i + 1) + CurrentTransform.GetTranslation()) * ScaleVector);
		}
		else
		{
			ProceduralVertices.Add((VertexBuffer->VertexPosition(i) + PrevTransform.GetTranslation()) * ScaleVector);
			ProceduralVertices.Add((VertexBuffer->VertexPosition(i + 1) + PrevTransform.GetTranslation()) * ScaleVector);
			SecondPartition.Add((VertexBuffer->VertexPosition(i + 2) + CurrentTransform.GetTranslation()) * ScaleVector);
			SecondPartition.Add((VertexBuffer->VertexPosition(i + 3) + CurrentTransform.GetTranslation()) * ScaleVector);
		}
	}
	ProceduralVertices.Append(SecondPartition);

	for (int i = 1; i < ProceduralVertices.Num() + 1; i++)
	{
		ProceduralTriangles.Add(i);
		(i % (ProceduralVertices.Num() / 2) == 0) ? ProceduralTriangles.Add(i - (ProceduralVertices.Num() / 2) + 1) : ProceduralTriangles.Add(i + 1);
		(i > ProceduralVertices.Num() / 2) ? ProceduralTriangles.Add(i % (ProceduralVertices.Num() / 2) + 1) : ProceduralTriangles.Add(i + ProceduralVertices.Num() / 2);
	}

	for (int i = 0; i < ProceduralVertices.Num(); i++)
	{
		ProceduralColors.Add(FColor::Red);
	}

	PaintMaterialInstance->ProceduralMeshComponent->CreateMeshSection(ProceduralSectionIndex, ProceduralVertices, ProceduralTriangles, TArray<FVector>(), TArray<FVector2D>(), ProceduralColors, TArray<FProcMeshTangent>(), false);
	ProceduralSectionIndex++;
}

void UPaintBrushComponent::ExportObj()
{
	FString dbgmsgstart = FString("Left hand grab, export initiated");
	FString dbgmsgsucceed = FString("Export successful");
	FString dbgmsgfail = FString("Error exporting-------------------------------------------------");

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, dbgmsgstart);

	// ensure we are using the right instanced static mesh
	ObjExporterInstance->RegisterStaticMeshComponent(PaintMaterialInstance->MeshComponent);

	if (ObjExporterInstance->ExportObjFile())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, dbgmsgsucceed);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsgfail);
	}
	//FString dbgmsg = FString(PaintMaterialInstance->MeshComponent->PerInstanceSMData.Last().Transform.ToString());

}