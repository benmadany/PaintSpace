// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Leap_NoPI.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APaintMaterial> PaintMaterial;
	
private:
	APaintMaterial* PaintMaterialInstance;
	Leap::Controller LeapController;
	int64_t PrevFrameID;

	void CheckHand(Leap::Frame frame);
	void TryPainting(Leap::Hand hand);
};
