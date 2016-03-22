// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Leap_NoPI.h"
#include "PainterController.generated.h"

/**
 * 
 */
UCLASS()
class PAINTSPACE_API APainterController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	APainterController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	void ProcessLeapFrame(Leap::Frame Frame);

	Leap::Controller LeapController;
	int64_t PrevFrameID;
	int64_t RightHandID;
	int64_t LeftHandID;
	
};
