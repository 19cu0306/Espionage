// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityCharacter.h"
#include "AI_SecurityCharacter.h"
#include "EspionageCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"

#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ASecurityCharacter::ASecurityCharacter()
	:isAlert(false)
	, bUTurn(false)
	, bPatrolLoop(false)
	,CharacterPrevLoc(FVector::ZeroVector)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Our sensing component to detect players by visibility and noise checks. */
	//PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnNoiseComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));
	bUseControllerRotationYaw = false;
}
// Called when the game starts or when spawned
void ASecurityCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ASecurityCharacter::OnHit);

	if (PatrolSpline != nullptr)
	{
		PatrolSplineComp = Cast<USplineComponent>(PatrolSpline->GetComponentByClass(USplineComponent::StaticClass()));
	}

	CharacterPrevLoc = GetActorLocation();
}
// Called every frame
void ASecurityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut)
	{
		bSensedTarget = false;
	}

	//emit footstep noise
	if (FVector::Dist(CharacterPrevLoc, GetActorLocation()) >= GetCharacterMovement()->MaxWalkSpeed/2)
	{
		CharacterPrevLoc = GetActorLocation();
		if (FootStepSound)
		{
			PlayCharacterSound(FootStepSound)->IsPlaying();
		}
	}
}
UAudioComponent* ASecurityCharacter::PlayCharacterSound(USoundCue* CueToPlay)
{
	if (CueToPlay)
	{
		return UGameplayStatics::SpawnSoundAttached(CueToPlay, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
	}
	return nullptr;
}

void ASecurityCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
								UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		Cast<AAI_SecurityCharacter>(GetController())->SetHitByPlayer(true);
	}
}

