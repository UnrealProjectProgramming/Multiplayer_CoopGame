# Multiplayer CoopGame 

## Personal Project

#### Engine Version 4.21.0

### A Coop multiplayer game made with ue4.

FIX THE GIF !

![Alt Text](https://media.giphy.com/media/9AI3BMj8FLat0Izea6/giphy.gif)
![Alt Text](https://i.imgur.com/DzUbr3t.png)


# What I did in this Project
  * Made the entire process of setting up a character
    * Made Character Class that Drives our player character such as Weapon system, Character movememnt and controls, death and     respawn..etc
      * Character Class includes: Setting up the player capsule.
      * Sprint Arm component for the camera.
      * Health Component to manage death sequence and taking damage.
      * Player input ( walk, run, jump, Zoom in and zoom out, crouch).
      * Handled Weapon spawning.
      * Replicating Some properties so it be MULTIPLAYER as well! and so on...
      
    * Setting up Abilities in C++ and Blueprints such as:
      * Health Regen.
      * Speed booster.
      * Both abilities are multiplayer ready.
      
    * Setting Up weapon in C++:
      * Attaching weapon to character.
      * Applying damage based on what part was hit in the body ( head shot, arm ... etc).
      * I use Line tracing to apply damage ( Ray casting ).
      * Simple camera shaking when we shoot.
      * Using custom collision channel and Physical material to apply Blood effect when we hit enemy or Flashy muzzle when we hit ground.
      * Automatic fire mode.
      * Created two weapons, normal rifle and a Grenade launcher that detonates on impact of after a specified time.
      * Simple Recoil using maths !
      * and it is multiplayer ready as well :)
      
    * Setting up Advanced Enemy A.I using Behaviour tree:
      * Using Unreal engine's Behaviour tree to make the Enemy A.I Behaviour.
      * Using EQS (Environment Query System) to Find nearest player, Take cover when taking damage, Regenerates its health.
      * Using Unreal Engine's Decorators, Services and tasks to manage the Behaviour Tree.
      * AI Perception so the Enemy A.I Can spot us.
      
    * Setting up Simple Tracker bot A.I using C++:
      * It basically like a tracking grenade that finds the players position, store it position then goes to this location when it reaches it searches for the player's position again and so on.
      * Detonates when it is near the player dealing Radial Damage that is determined by how close the player(s) is to the TrackerBot.
      
    * Wave Spawning system, i.e spawning enemies then wait for next wave then spawn more enemies and so on, Logic made in C++ and blueprint.
    
    * Player state to keep every player score consistent on the server. 


## Here is a gameplay video.
[Gameplay Video](https://youtu.be/IJNvPF_M1Js)
________________________________________________________________________________________________________________________________________
## Here are some Gifs, pics, Important code snippits and blueprints that shows the highlights of the things I did :)

![Alt Text](https://i.imgur.com/DzUbr3t.png)

![Alt Text](https://i.imgur.com/xamghP9.png)    

________________________________________________________________________________________________________________________________________

### A.I Behaviour Tree
![Alt Text](https://i.imgur.com/crEt5lX.png)

![Alt Text](https://i.imgur.com/DeOxhhM.png)

________________________________________________________________________________________________________________________________________

### Weapon Properties
![Alt Text](https://i.imgur.com/OAHYxrT.png)


________________________________________________________________________________________________________________________________________

### Power ups Code snippit
```cpp
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
```

![Alt Text](https://i.imgur.com/W9vkzo8.png)


________________________________________________________________________________________________________________________________________

### Spawning and Counting Score which is consistent for the players
![Alt Text](https://i.imgur.com/zpDqEdu.png)

![Alt Text](https://i.imgur.com/o0Ytrgk.png)


________________________________________________________________________________________________________________________________________

### Custom physical Surface type 
![Alt Text](https://i.imgur.com/DSI8cXe.png)

### That is how damage is calculated using Phys material (Custom physical surface type from the previous pic), we basically check what is the phy material coming back from the populated "Hit" when we make the line tracing, then we decide what damage to apply, whether we deal more damage , play certain effect, sound or add damage multipier... etc

![Alt Text](https://i.imgur.com/0HIADnP.png)

![Alt Text](https://i.imgur.com/rW0Cz6H.png)

________________________________________________________________________________________________________________________________________

## Simple A.I Tracker bot that exploades when near the player

![Alt Text](https://media.giphy.com/media/3PxSQPwAtJ89FPU6ew/giphy.gif)

#### Determining rolling volume based on velocity
![Alt Text](https://i.imgur.com/8Mn4Wgi.png)

```cpp
static int32 DebugTrackerBotDrawing = 0;
FAutoConsoleVariableRef CVARDebugTrackerBotDrawing
(TEXT("COOP.DebugTrackerBot"),
	DebugTrackerBotDrawing,
	TEXT("Draw Debug Lines for TrackerBot"),
	ECVF_Cheat);



// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionDamage = 60;
	ExplosionRadius = 350	;
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
	AActor* BestTarget = nullptr;
	float NearestTargetDistance = FLT_MAX;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || USHealthComponent::IsFriendly(TestPawn, this))
		{
			continue;
		}

		USHealthComponent* TestPawnHealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (TestPawnHealthComp && TestPawnHealthComp->GetHealth() > 0)
		{
			float Distance = (TestPawn->GetActorLocation() - GetActorLocation()).Size();
			if (NearestTargetDistance > Distance)
			{
				BestTarget = TestPawn;
				NearestTargetDistance = Distance;
			}
		}
	}


	// To use UNavigationSystemV1 this we must add NavigationSystem in the CoopGame.Build.cs module.

	if (BestTarget)
	{
		auto NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget, 50.0f);
		
		GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);
		GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &ASTrackerBot::RefreshPath, 5.0f, false);

		if (!NavPath) { return FVector::ZeroVector; }

		if (NavPath->PathPoints.Num() > 1)
		{
			// Return next point in the path
			return NavPath->PathPoints[1];
		}
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
```

________________________________________________________________________________________________________________________________________

## Key Bindings
![Alt Text](https://i.imgur.com/9uByBzf.png)


________________________________________________________________________________________________________________________________________
