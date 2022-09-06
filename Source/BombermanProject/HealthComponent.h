// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBERMANPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	UPROPERTY(EditAnywhere)
	float currentHealth;
	UPROPERTY(EditAnywhere)
	float maxHealth;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return currentHealth; }
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() { return maxHealth; }
	void SetHealth(float health) { currentHealth = health; }
	void SetMaxHealth(float health) { maxHealth = health; }
	void TakeDamage(float damageAmount) { currentHealth -= damageAmount; }
	 
		
};
