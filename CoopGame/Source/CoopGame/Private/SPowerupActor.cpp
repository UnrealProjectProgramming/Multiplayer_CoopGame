// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"

#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TotalNumberOfTicks = 0;

	PowerupInterval = 0.0f;

	SetReplicates(true);
}


void ASPowerupActor::OnTickPowerup(AActor* ActivateForActor)
{
	TicksProcessed++;
	OnPowerupTicked(ActivateForActor);

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



void ASPowerupActor::ActivatePowerup(AActor* ActivateForActor)
{
	OnActivated(ActivateForActor);
	
	bIsPowerupActive = true;
	OnRep_PowerupActive();

	if (PowerupInterval > 0.0f)
	{
		/*
			using timer delegete so we can be able to Bind a UFUNCTION,
			by doing this we can pass the function with parameters 
		*/
		FTimerDelegate TimerDelegate_OnTickPowerup;
		TimerDelegate_OnTickPowerup.BindUFunction(this, FName("OnTickPowerup"), ActivateForActor);

		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, TimerDelegate_OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup(ActivateForActor);
	}
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
}