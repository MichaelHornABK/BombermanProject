// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include <string>

void UPlayerUIWidget::SetHealth(float currentHealth, float maxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(currentHealth / maxHealth);
	}
	if (HealthText)
	{
		std::string currHealth = std::to_string(currentHealth);
		FString formatHealth = UTF8_TO_TCHAR(currHealth.c_str());
		HealthText->SetText(FText::FromString(formatHealth));
	}
}
