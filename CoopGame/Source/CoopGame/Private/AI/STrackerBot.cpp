// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "SHealthComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"
#include "SCharacter.h"
#include "Sound/SoundCue.h"



// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionDamage = 100.0f;
	ExplosionRadius = 200.0f;
	MovementForce = 1000.0f;
	Accurecy = 50.0f;
	bUseVelocityChange = true;
	SelfDestructInterval = 0.5f;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(ExplosionRadius);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SetReplicates(true);

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();
	}
}


FVector ASTrackerBot::GetNextPathPoint()
{

	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerPawn) { return FVector::ZeroVector; }

	// To use UNavigationSystemV1 this we must add NavigationSystem in the CoopGame.Build.cs module.
	auto NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn, 50.0f);

	if (!NavPath) { return FVector::ZeroVector; }

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
	if (Role == ROLE_Authority)
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
}


void ASTrackerBot::HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Expload ! 

	if (Health <= 0)
	{
		SelfDestruct();
	}

	if (MatInstDynamic == nullptr)
	{
		MatInstDynamic = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInstDynamic)
	{
		MatInstDynamic->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	UE_LOG(LogTemp, Warning, TEXT("health changed: %s  by  %s"), *FString::SanitizeFloat(Health), *GetName());
}


void ASTrackerBot::SelfDestruct()
{
	if (bExploaded)
	{
		return;
	}

	bExploaded = true;

	UGameplayStatics::SpawnEmitterAtLocation(this, ExploasinEffect, GetActorLocation());

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), false);
	UGameplayStatics::PlaySoundAtLocation(this, ExploasionSound, GetActorLocation());

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Purple, true, 5.0);

	Destroy();
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20.0, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (bStartedSelfDestruction)
	{
		return;
	}

	ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
	if (PlayerPawn)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDestructInterval, true, 0.0f);
		bStartedSelfDestruction = true;

		UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
	}
}
