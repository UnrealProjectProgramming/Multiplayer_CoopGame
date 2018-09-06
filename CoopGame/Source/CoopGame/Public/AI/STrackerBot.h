// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USphereComponent;
class USoundCue;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()



public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

	UFUNCTION()
	FVector GetNextPathPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveToPlayer();

	void SelfDestruct();

	void DamageSelf();

	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float Accurecy;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDestructInterval;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExploasinEffect;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* ExploasionSound;

	bool bExploaded;

	FVector NextPathPoint;

	UMaterialInstanceDynamic* MatInstDynamic;

	FTimerHandle TimerHandle_SelfDamage;

	FTimerHandle TimerHandle_RefreshPath;

	bool bStartedSelfDestruction;

	void RefreshPath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	
};
