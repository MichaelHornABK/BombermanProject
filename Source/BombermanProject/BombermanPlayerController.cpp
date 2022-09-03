// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanPlayerController.h"
#include "BombermanPlayerCharacter.h"

void ABombermanPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ABombermanPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAxis(TEXT("MoveForward"), this, &ABombermanPlayerController::RequestMoveForward);
		InputComponent->BindAxis(TEXT("MoveRight"), this, &ABombermanPlayerController::RequestMoveRight);
		InputComponent->BindAction(TEXT("DropBomb"), EInputEvent::IE_Pressed, this, &ABombermanPlayerController::RequestDropObject);

	}
}

void ABombermanPlayerController::RequestMoveForward(float AxisValue)
{

	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), AxisValue);
	}
}

void ABombermanPlayerController::RequestMoveRight(float AxisValue)
{

	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), AxisValue);
	}
}

void ABombermanPlayerController::RequestDropObject()
{
	if (ABombermanPlayerCharacter* BombermanPlayer = Cast<ABombermanPlayerCharacter>(GetCharacter()))
	{
		BombermanPlayer->RequestDropObject();
	}
}