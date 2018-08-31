// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"

#include "SHealthComponent.h"
#include "TimerManager.h"
#include "SGameState.h"


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
	GameStateClass = ASGameState::StaticClass();

	TimeBetweenWaves = 5;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5;
}


void ASGameMode::StartPlay()
{
	Super::StartPlay();

	SetWaveState(EWaveState::StartGame);

	PrepareForNextWave();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
	CheckAnyPlayerAlive();
}

void ASGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameMode::SpawnBotTimerElapsed, 2.0, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
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
	SetWaveState(EWaveState::WaitingToComplete);
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
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}

}


void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}

	// All players are dead. 

	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);

	// @TODO: Finish up the match and present game over to the player along with game stats.

	UE_LOG(LogTemp, Warning, TEXT("Game Over: Owari da"));
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GM = GetGameState<ASGameState>();
	if (ensureAlways(GM))
	{
		GM->SetWaveState(NewState);
	}
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
	SetWaveState(EWaveState::WaitingForStart);
}
