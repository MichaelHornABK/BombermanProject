// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanPlayerCharacter.h"

// Sets default values
ABombermanPlayerCharacter::ABombermanPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABombermanPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABombermanPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABombermanPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABombermanPlayerCharacter::OnBombAttached(AUsableBomb* InUsableBomb)
{
	UsableBomb = InUsableBomb;
	MoveIgnoreActorAdd(UsableBomb);
	//ClientThrowableAttached(InUsableBomb);
}


void ABombermanPlayerCharacter::ResetThrowableObject()
{
	//drop object
	if (UsableBomb)
	{
		UsableBomb->Drop();
	}
	UsableBomb = nullptr;
}

void ABombermanPlayerCharacter::RequestDropObject()
{
	if (UsableBomb)
	{
		UsableBomb->Drop();
	}
	UsableBomb = nullptr;
}