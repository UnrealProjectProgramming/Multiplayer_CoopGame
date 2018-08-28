// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class USphereComponent;
class UDecalComponent;

class ASPowerupActor;

UCLASS()
class COOPGAME_API ASPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupActor();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Respawn();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent * SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "PowerupSpawnClass")
	TSubclassOf<ASPowerupActor> PowerupClass;

	/* Powerup cool down in seconds */
	UPROPERTY(EditDefaultsOnly, Category = "PowerupSpawnClass")
	float CoolDownDuration;

	ASPowerupActor* CurrentPowerupInstance;

	FTimerHandle TimerHandle_Respawn;
	
};
