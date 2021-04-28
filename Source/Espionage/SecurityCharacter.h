// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "SecurityCharacter.generated.h"

class USoundCue;

UCLASS()
class ESPIONAGE_API ASecurityCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASecurityCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isAlert;
	bool bUTurn;

	UPROPERTY(EditAnywhere, Category = "Patrol Spline")
		bool bPatrolLoop;
	UPROPERTY(EditAnywhere, Category = "Patrol Spline")
		AActor* PatrolSpline;

	//UPROPERTY(VisibleAnywhere, Category = "AI")
		//class UPawnSensingComponent* PawnSensingComp;		//legacy
	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, Category = "AI")
		class UPawnNoiseEmitterComponent* PawnNoiseComp;

	UPROPERTY(EditAnywhere, Category = "SOUND")
		USoundCue* FootStepSound;

	UFUNCTION()
		class UAudioComponent* PlayCharacterSound(USoundCue* CueToPlay);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SenseTimeOut;

	/* Last time the player was spotted */
	float LastSeenTime;
	/* Last time the player was heard */
	float LastHeardTime;


	/* Resets after sense time-out to avoid unnecessary clearing of target each tick */
	bool bSensedTarget;

	//use to emit sound
	FVector CharacterPrevLoc;

	class USplineComponent* PatrolSplineComp;
};
