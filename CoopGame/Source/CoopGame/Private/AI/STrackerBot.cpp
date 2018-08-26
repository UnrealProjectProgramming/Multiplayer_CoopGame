// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"



// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetCanEverAffectNavigation(false);
	RootComponent = MeshComp;

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ASTrackerBot::GetNextPathPoint()
{

	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerPawn) { return FVector::ZeroVector; }

	// To use UNavigationSystemV1 this we must add NavigationSystem in the CoopGame.Build.cs module.
	auto NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn, 50.0f);

	if (NavPath->PathPoints.Num() > 1)
	{
		// Return next point in the path
		return NavPath->PathPoints[1];
	}
	// if it fails return actor location!
	return GetActorLocation();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
