// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Leap_NoPI.h"
#include "StaticMeshResources.h"
#include "ObjExport/ObjExporter.h" // temp
#include "PaintBrushComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAINTSPACE_API UPaintBrushComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPaintBrushComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "Instanced Static Mesh")
	void ClearAllStrokes();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APaintMaterial> PaintMaterial; // template for PaintMaterialInstance
	
private:
	void ProcessLeapFrame(Leap::Frame Frame, float DeltaSeconds);
	void Paint();
	void GenerateProceduralMesh(FInstancedStaticMeshInstanceData PrevMesh, FInstancedStaticMeshInstanceData CurrentMesh);
	void ExportObj();

	Leap::Controller LeapController;
	int64_t PrevFrameID;
	float Delay;
	APaintMaterial* PaintMaterialInstance;
	FStaticMeshLODResources* LODModel;
	FPositionVertexBuffer* VertexBuffer;
	FVector PreviousLocation;
	int64_t ProceduralSectionIndex;
	//FName IndexFingerSocket;
	ObjExporter* ObjExporterInstance;
};
