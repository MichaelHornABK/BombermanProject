// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANPROJECT_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetHealth(float currentHealth, float maxHealth);


	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* HealthText;
};
