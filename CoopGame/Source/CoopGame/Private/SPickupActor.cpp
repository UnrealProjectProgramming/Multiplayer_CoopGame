// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor.h"

#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
ASPickupActor::ASPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComp;
	SphereComp->SetSphereRadius(75.0f);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));
	DecalComp->DecalSize = FVector(65, 75, 75);
}


// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// @TODO : add power ups ! 
}



