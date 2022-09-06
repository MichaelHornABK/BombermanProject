// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UsableBomb.h"
#include "BombermanPlayerCharacter.generated.h"

class UHealthComponent;
UCLASS()
class BOMBERMANPROJECT_API ABombermanPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABombermanPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnBombAttached(AUsableBomb* InUsableBomb);
	void ResetThrowableObject();
	void RequestDropObject();
	UPROPERTY(EditAnywhere);
	UHealthComponent* playerHealth;
	UFUNCTION(BlueprintCallable)
	float GetPlayerHealth();
	UFUNCTION(BlueprintCallable)
	float GetPlayerMaxHealth();
 private:
	UPROPERTY()
		AUsableBomb* UsableBomb;
};
