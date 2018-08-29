// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

	void ActivatePowerup(AActor* ActivateForActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivated(AActor* ActivateForActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupTicked(AActor* ActivateForActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpierd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerStateChanged(bool bNewIsActive);

	UFUNCTION()
	void OnTickPowerup(AActor* ActivateForActor);

protected:
	
	UFUNCTION()
	void OnRep_PowerupActive();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float PowerupInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNumberOfTicks;

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
	bool bIsPowerupActive;

	int32 TicksProcessed;

	FTimerHandle TimerHandle_PowerupTick;

};
