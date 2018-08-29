// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor.h"

#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"

#include "SPowerupActor.h"

// Sets default values
ASPickupActor::ASPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CoolDownDuration = 5.0f;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComp;
	SphereComp->SetSphereRadius(75.0f);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));
	DecalComp->DecalSize = FVector(65, 75, 75);

	SetReplicates(true);
}


// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		Respawn();
	}
}


void ASPickupActor::Respawn()
{
	
	if (!PowerupClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s, please update your blueprint."), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentPowerupInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerupClass, GetTransform(), SpawnParams);
	
}


void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role==ROLE_Authority && CurrentPowerupInstance)
	{
		CurrentPowerupInstance->ActivatePowerup(OtherActor);
		CurrentPowerupInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPickupActor::Respawn, CoolDownDuration);
	}
}



