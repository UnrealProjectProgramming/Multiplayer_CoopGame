// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

// Forward Declarations

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void BeginJump();

	void BeginZoom();

	void EndZoom();
	
	void StartFire();

	void StopFire();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent * CameraComp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;


	bool bWantsToZoom;


	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;


	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.0f, ClampMax = 100.0f))
	float ZoomInterpSpeed = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	/* Default FOV which is set during the gameplay ! */
	float DefaultFOV;

	ASWeapon* CurrentWeapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual FVector GetPawnViewLocation() const override;

	
};
