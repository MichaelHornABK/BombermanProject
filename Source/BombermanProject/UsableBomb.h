// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsableBomb.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class BOMBERMANPROJECT_API AUsableBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsableBomb();

	UFUNCTION(BlueprintCallable)
		bool IsIdle() const { return State == EState::Idle; }

	UFUNCTION(BlueprintCallable)
		bool Pull(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		void Launch(const FVector& InitialVelocity, AActor* Target = nullptr);

	UFUNCTION(BlueprintCallable)
		void Drop();

	UFUNCTION(BlueprintCallable)
		void ToggleHighlight(bool bIsOn);

	FTimerHandle BombTimerHandle;
protected:
	enum class EState
	{
		Idle,
		Pull,
		Attached,
		Launch,
		Dropped,
	};
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION()
		void ProjectileStop(const FHitResult& ImpactResult);

	UFUNCTION(BlueprintCallable)
		bool SetHomingTarget(AActor* Target);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectileMovementComponent;

	EState State = EState::Idle;

	UPROPERTY()
		AActor* PullActor = nullptr;

	void ActivateBomb();
	void Explode();
	void DestroyObject();

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* NS_Explosion;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* NS_Fuse;

	UPROPERTY(EditAnywhere)
		UAudioComponent* SB_Fuse;

	UPROPERTY(EditAnywhere)
		UAudioComponent* SB_Explosion;

};
