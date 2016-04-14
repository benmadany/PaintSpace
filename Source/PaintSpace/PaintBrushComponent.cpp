// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "PaintBrushComponent.h"
#include "PaintMaterial.h"

//using namespace Leap;

// This class should be attached to some scene component in order to provide paint functionality based on sockets.

// Sets default values for this component's properties
UPaintBrushComponent::UPaintBrushComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	PrevFrameID = -1;
	ProceduralSectionIndex = 0;

	ObjExporter ObjExp = ObjExporter();
	ObjExporterInstance = &ObjExp;

	PreviousLocation = FVector(0, 0, 0);

	Delay = 0.0f;
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
			//PaintMaterialInstance->ProceduralMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//PaintMaterialInstance->ProceduralMeshComponent->SetMaterial(0, PaintMaterialInstance->InstanceMesh->Materials[0]->GetMaterial());
			LODModel = &PaintMaterialInstance->MeshComponent->StaticMesh->RenderData->LODResources[0];
			VertexBuffer = &LODModel->PositionVertexBuffer;
		}
	}

	/*if (GEngine->HMDDevice.IsValid())
	{
		//IHeadMountedDisplay* HMD = GEngine->HMDDevice.Get();
		FString vrmsg = FString(TEXT("HMD Detected, optimizing controls."));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, vrmsg);
		LeapController.setPolicy(Controller::PolicyFlag::POLICY_OPTIMIZE_HMD);
	}*/
}


// Called every frame
void UPaintBrushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LeapController.isConnected())
	{
		const Leap::Frame LatestFrame = LeapController.frame();
		if (LatestFrame.id() != PrevFrameID)
		{
			ProcessLeapFrame(LatestFrame, DeltaTime);

			PrevFrameID = LatestFrame.id();
		}

	}

}

void UPaintBrushComponent::ClearAllStrokes()
{
	if (PaintMaterialInstance)
	{
		PaintMaterialInstance->MeshComponent->ClearInstances();
	}
}


void UPaintBrushComponent::ExportObjBP()
{
	ExportObj();
}


void UPaintBrushComponent::ProcessLeapFrame(Leap::Frame Frame, float DeltaSeconds)
{
	for (Leap::Hand Hand : Frame.hands())
	{
		if (Hand.isRight())
		{
			Leap::Finger Thumb;
			Leap::Finger Index;

			for (Leap::Finger Finger : Hand.fingers())
			{
				if (Finger.type() == Leap::Finger::Type::TYPE_THUMB)
				{
					Thumb = Finger;
				}
				if (Finger.type() == Leap::Finger::Type::TYPE_INDEX)
				{
					Index = Finger;
				}
			}
			if (Thumb.isValid() && Index.isValid())
			{
				if (!Thumb.isExtended() && Index.isExtended() && Hand.grabStrength() < 0.2f)
				{
					if (Delay < 0.5f)
					{
						Delay += DeltaSeconds;
					}
					else {
						Paint();
					}
				}
				else
				{
					Delay = 0.0f;
				}
			}
		}
	}
}


void UPaintBrushComponent::Paint()
{
	int32 instances = PaintMaterialInstance->MeshComponent->PerInstanceSMData.Num();

	const FVector SpawnLocation = GetComponentLocation();

	FRotator SpawnRotation = FRotator(0,0,0);

	if (instances > 0)
	{
		FVector Direction = SpawnLocation - PreviousLocation;
		// MakeFromZ because we only rotate about Z axis (yaw)
		SpawnRotation = FRotationMatrix::MakeFromZ(Direction).Rotator();
	}

	// spawn static mesh
	FVector ScaleVector = FVector(0.01f, 0.01f, 0.05f); // experimentally determined
	PaintMaterialInstance->MeshComponent->AddInstance(FTransform(SpawnRotation, SpawnLocation, ScaleVector));

	/*if (instances > 1)
	{
		GenerateProceduralMesh(PaintMaterialInstance->MeshComponent->PerInstanceSMData[instances - 2], PaintMaterialInstance->MeshComponent->PerInstanceSMData[instances - 1]);
	}*/
	//FString dbgmsg = FString(PaintMaterialInstance->MeshComponent->PerInstanceSMData.Last().Transform.ToString());

	//FString dbgmsg = FString((SpawnLocation.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, dbgmsg);

	PreviousLocation = SpawnLocation;

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
			ProceduralVertices.Add(VertexBuffer->VertexPosition(i + 2) * ScaleVector + PrevTransform.GetTranslation());
			ProceduralVertices.Add(VertexBuffer->VertexPosition(i + 3) * ScaleVector + PrevTransform.GetTranslation());
			SecondPartition.Add(VertexBuffer->VertexPosition(i) * ScaleVector + CurrentTransform.GetTranslation());
			SecondPartition.Add(VertexBuffer->VertexPosition(i + 1) * ScaleVector + CurrentTransform.GetTranslation());
		}
		else
		{
			ProceduralVertices.Add(VertexBuffer->VertexPosition(i) * ScaleVector + PrevTransform.GetTranslation());
			ProceduralVertices.Add(VertexBuffer->VertexPosition(i + 1) * ScaleVector + PrevTransform.GetTranslation());
			SecondPartition.Add(VertexBuffer->VertexPosition(i + 2) * ScaleVector + CurrentTransform.GetTranslation());
			SecondPartition.Add(VertexBuffer->VertexPosition(i + 3) * ScaleVector + CurrentTransform.GetTranslation());
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
	FString dbgmsgsucceed = FString("Export successful");
	FString dbgmsgfail = FString("-Error exporting-");


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

}