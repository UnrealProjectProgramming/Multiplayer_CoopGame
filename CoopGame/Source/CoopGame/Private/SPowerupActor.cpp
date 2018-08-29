// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TotalNumberOfTicks = 0;

	PowerupInterval = 0.0f;

	SetReplicates(true);
}


void ASPowerupActor::OnTickPowerup()
{
	TicksProcessed++;
	OnPowerupTicked();

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		OnExpierd();

		bIsPowerupActive = false;
		OnRep_PowerupActive();

		// Delete Timer !
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}


void ASPowerupActor::OnRep_PowerupActive()
{
	OnPowerStateChanged(bIsPowerupActive);
}



void ASPowerupActor::ActivatePowerup()
{
	OnActivated();
	
	bIsPowerupActive = true;
	OnRep_PowerupActive();

	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
}