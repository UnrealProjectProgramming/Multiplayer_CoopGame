// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShake;


USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	// we converd the physical surface to  TEnum As byte so It can be saved in the struct and can be replicated.
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:

	void PlayFireEffects(FVector TracerEndPoint);

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_HitScanTrace();


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float BaseDamage;

	/* Bullet Spread in Degrees*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float HeadShotDamageMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> CameraShakeClass;

	/*The weapon rate of fire, The higher the value is the faster the weapon will shot the Ray (Bullet) */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 1.0f, ClampMax = 2000.0f))
	float RateOfFire;

	float TimeBetweenShots;

	float LastFireTime;
	
	FTimerHandle TimeHandle_TimeBetweenShots;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;
public:

	
	virtual void Fire();

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	void StartFire();

	void StopFire();
};
