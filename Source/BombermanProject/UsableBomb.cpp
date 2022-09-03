// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableBomb.h"
#include "Components/StaticMeshComponent.h"
#include "BombermanPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"

// Sets default values
AUsableBomb::AUsableBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	RootComponent = StaticMeshComponent;
	SB_Fuse = CreateDefaultSubobject<UAudioComponent>(TEXT("FuseSound"));
	SB_Explosion = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	
}

// Called when the game starts or when spawned
void AUsableBomb::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AUsableBomb::ProjectileStop);
	}
}

void AUsableBomb::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		ProjectileMovementComponent->OnProjectileStop.RemoveDynamic(this, &AUsableBomb::ProjectileStop);
	}
	Super::EndPlay(EndPlayReason);
}

void AUsableBomb::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (ABombermanPlayerCharacter* BombermanCharacter = Cast<ABombermanPlayerCharacter>(Other))
	{
		if (this->Owner != BombermanCharacter)
		{
			
			AttachToComponent(BombermanCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ObjectAttach"));
			SetOwner(BombermanCharacter);
			ProjectileMovementComponent->Deactivate();
			//set character state to attached
			BombermanCharacter->OnBombAttached(this);
		}
	}
}

void AUsableBomb::ProjectileStop(const FHitResult& ImpactResult)
{
	if (State == EState::Launch || State == EState::Dropped)
	{
		State = EState::Idle;
	}
}

bool AUsableBomb::Pull(AActor* InActor)
{
	if (State != EState::Idle)
	{
		return false;
	}

	if (SetHomingTarget(InActor))
	{
		ToggleHighlight(false);
		State = EState::Pull;
		PullActor = InActor;
		return true;
	}

	return false;
}

void AUsableBomb::Launch(const FVector& InitialVelocity, AActor* Target /* = nullptr */)
{
	if (State == EState::Pull || State == EState::Attached)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		ProjectileMovementComponent->Activate(true);
		ProjectileMovementComponent->HomingTargetComponent = nullptr;

		State = EState::Launch;

		if (Target)
		{
			if (USceneComponent* SceneComponent = Cast<USceneComponent>(Target->GetComponentByClass(USceneComponent::StaticClass())))
			{
				ProjectileMovementComponent->HomingTargetComponent = TWeakObjectPtr<USceneComponent>(SceneComponent);
				return;
			}
		}

		ProjectileMovementComponent->Velocity = InitialVelocity;
	}
}

void AUsableBomb::Drop()
{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ActivateBomb();
}

void AUsableBomb::ToggleHighlight(bool bIsOn)
{
	StaticMeshComponent->SetRenderCustomDepth(bIsOn);
}

bool AUsableBomb::SetHomingTarget(AActor* Target)
{
	if (Target)
	{
		if (USceneComponent* SceneComponent = Cast<USceneComponent>(Target->GetComponentByClass(USceneComponent::StaticClass())))
		{
			if (USceneComponent* ThrowableSceneComponent = Cast<USceneComponent>(GetComponentByClass(USceneComponent::StaticClass())))
			{
				ProjectileMovementComponent->SetUpdatedComponent(ThrowableSceneComponent);
				ProjectileMovementComponent->Activate(true);
				ProjectileMovementComponent->HomingTargetComponent = TWeakObjectPtr<USceneComponent>(SceneComponent);
				ProjectileMovementComponent->Velocity = FVector(0.0f, 0.0f, 1000.0f);
				return true;
			}
		}
	}

	return false;
}

void AUsableBomb::ActivateBomb()
{
	FVector BombLocation = StaticMeshComponent->GetComponentLocation();

	if (NS_Fuse)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Fuse, BombLocation);
	}

	

	if (SB_Fuse)
	{
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Fuse, BombLocation);
		SB_Fuse->Activate();
	}

	GetWorldTimerManager().SetTimer(BombTimerHandle, this, &AUsableBomb::Explode, 5.0f, false);
}

void AUsableBomb::Explode()
{
	FVector BombLocation = StaticMeshComponent->GetComponentLocation();
	if (NS_Explosion)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Explosion, BombLocation);
	}
	if (SB_Explosion)
	{
		SB_Explosion->Activate();
	}
	TArray<FHitResult> OutHits;
	FVector Start = BombLocation;
	FVector End = BombLocation;

	FCollisionShape BombColSphere = FCollisionShape::MakeSphere(500.0f);

	DrawDebugSphere(GetWorld(), GetActorLocation(), BombColSphere.GetSphereRadius(), 50, FColor::Cyan, true);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_WorldDynamic, BombColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((Hit.GetActor())->GetRootComponent());

			if (MeshComp)
			{
				MeshComp->AddRadialImpulse(GetActorLocation(), 500.f, 2000.f, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
	
	GetWorldTimerManager().SetTimer(BombTimerHandle, this, &AUsableBomb::DestroyObject, 1.0f, false);
}

void AUsableBomb::DestroyObject()
{
	this->Destroy();
}