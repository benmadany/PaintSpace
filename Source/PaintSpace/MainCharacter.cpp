// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	UCameraComponent* VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));

	VRCamera->AttachTo(RootComponent);
	VRCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 95.0f));
	VRCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	VisibleComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

