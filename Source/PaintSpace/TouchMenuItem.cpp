// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "TouchMenuItem.h"


ATouchMenuItem::ATouchMenuItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Opacity = 0.0f;
	Offset = FVector(0, 0, 0);
}

void ATouchMenuItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (!RootComponent)
	{
		// Need to have root component assigned
		Destroy();
	}
	DynamicMat = UMaterialInstanceDynamic::Create(GetRootSMC()->GetMaterial(0), this);
	GetRootSMC()->SetMaterial(0, DynamicMat);
}

void ATouchMenuItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


UStaticMeshComponent* ATouchMenuItem::GetRootSMC()
{
	return Cast<UStaticMeshComponent>(RootComponent);
}


void ATouchMenuItem::UpdateOpacity(float NewOpacity)
{
	Opacity = NewOpacity;
	DynamicMat->SetScalarParameterValue("Opacity", Opacity);
}


FVector ATouchMenuItem::GetLocationOffset()
{
	return Offset;
}


void ATouchMenuItem::SetLocationOffset(FVector NewOffset)
{
	Offset = NewOffset;
}