// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BombermanPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANPROJECT_API ABombermanPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

protected:
	void SetupInputComponent() override;
	void RequestMoveForward(float AxisValue);
	void RequestMoveRight(float AxisValue);
	void RequestDropObject();
};
