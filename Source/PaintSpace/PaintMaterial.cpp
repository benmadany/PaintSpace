// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "PaintMaterial.h"


// Sets default values
APaintMaterial::APaintMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APaintMaterial::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APaintMaterial::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

