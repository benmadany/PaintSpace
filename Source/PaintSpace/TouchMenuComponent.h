// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Leap_NoPI.h"
#include "TouchMenuItem.h"
#include "TouchMenuComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PAINTSPACE_API UTouchMenuComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UTouchMenuComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "Helper")
	void BPExpandMenu();


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Tree")
	//UTouchMenuComponent* ParentTMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Children")
	TArray<TSubclassOf<ATouchMenuItem>> ChildTemplates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Item")
	TSubclassOf<ATouchMenuItem> MenuItemTemplate;

private:
	void CheckLeapFrame();
	void DetermineVisibility();
	void DetermineLocation();
	void ExpandMenu();
	void CollapseMenu();

	Leap::Controller LeapController;
	int64_t PrevFrameID;
	ATouchMenuItem* MenuItem;
	TArray<ATouchMenuItem*> Children;
	//UMaterialInstanceDynamic* DynamicMat;
	
	float RootOpacity;
	float ChildOpacity;
	bool Enabled;
	bool PalmFacingPlayer;
	bool ShowMenu;
	bool HasLeftHand;
	bool MenuOpen;
};
