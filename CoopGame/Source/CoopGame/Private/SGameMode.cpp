// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"

#include "SHealthComponent.h"
#include "TimerManager.h"

/*
	Important Notes: GAME MODE
	The GameModeBase defines the game being played. 
	It governs the game rules, scoring, what actors are allowed to exist in this game type, and who may enter the game.
	It is only instanced on the server and will never exist on the client.
*/


/*
	Important Notes: Game State
	Game State holds all of our replicated information about the current game and that is because we can not put that in game mode
	because as we know game mode only exist on the server and clients do not actually have a copy of game mode, 
	that means whenever we want to replicate something we have 
	to put that in a separate class which is GameState.
*/


/*
	Important Notes: Player State
	That contains all of the persistent information of a player and that is because the player controller also does not exist on other clients
	it only exist on YOUR MACHINE and on the server.
*/

ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 5;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}


void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}

void ASGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameMode::SpawnBotTimerElapsed, 2.0, true, 0.0f);
}


void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawn--;

	if (NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnBot);
}


void ASGameMode::CheckWaveState()
{

	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NumberOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	bool bIsAnybotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}
		
		USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthComp && HealthComp->GetHealth() > 0)
		{
			bIsAnybotAlive = true;
			break;
		}
	}

	if (!bIsAnybotAlive)
	{
		PrepareForNextWave();
	}

}


void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}
