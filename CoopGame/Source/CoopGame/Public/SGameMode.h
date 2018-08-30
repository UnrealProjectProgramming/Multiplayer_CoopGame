// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode();

	/** Transitions to calls BeginPlay on actors. */
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;
protected:

	UPROPERTY(EditDefaultsOnly, Category = "SGameMode")
	float TimeBetweenWaves;

	FTimerHandle TimerHandle_SpawnBot;

	FTimerHandle TimerHandle_NextWaveStart;

	int32 NumberOfBotsToSpawn;

	int32 WaveCount;
	
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "SGameMode")
	void SpawnNewBot();

	// Start Spawning Bot and start a timer
	void StartWave();

	// Stop spawning bots and reset Timer
	void EndWave();

	// Set Timer for next wave
	void PrepareForNextWave();

	void CheckWaveState();
	
	UFUNCTION()
	void SpawnBotTimerElapsed();
};
