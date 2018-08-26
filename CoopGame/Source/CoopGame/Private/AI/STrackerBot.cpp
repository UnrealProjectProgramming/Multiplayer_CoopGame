// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"



// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;


	MovementForce = 1000.0f;
	Accurecy = 50.0f;
	bUseVelocityChange = true;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	NextPathPoint = GetNextPathPoint();
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

	MoveToPlayer();
}

void ASTrackerBot::MoveToPlayer()
{
	float DistanceToPlayer = FVector::Distance(GetActorLocation(), NextPathPoint);

	if (DistanceToPlayer <= Accurecy)
	{
		NextPathPoint = GetNextPathPoint();

		DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached !", this, FColor::Cyan);
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= MovementForce;

		MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + NextPathPoint, 36, FColor::Green, false, 0.0f);
	}

	DrawDebugSphere(GetWorld(), NextPathPoint, 32, 12, FColor::Red, false, 0.0f);
}