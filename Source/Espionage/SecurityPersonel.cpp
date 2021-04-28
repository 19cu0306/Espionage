// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityPersonel.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Detection.h"

// Sets default values
ASecurityPersonel::ASecurityPersonel()
	:defaultSpeed(1)
	, bLoop(false)
	, turnRate(1.f)
	, m_localLength(0.f)
	, lenghOnSpline(0)
	, overallLength(0)
	, uTurn(false)
	, pTargetSpline(NULL)
	, AnimaState(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComp");

	pSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	pSkeletalMesh->SetupAttachment(RootComponent);
	pSkeletalMesh->OnComponentHit.AddDynamic(this, &ASecurityPersonel::OnActorHit);

	DetectionComp = CreateDefaultSubobject<UDetection>("Detection");
	DetectionComp->DetecMesh->SetupAttachment(pSkeletalMesh);

}

// Called when the game starts or when spawned
void ASecurityPersonel::BeginPlay()
{
	Super::BeginPlay();

	if (DetectionComp != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detect Comp is Inited"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("Spline Init"));
	if (pSplineActor != nullptr)
	{
		pTargetSpline = Cast<USplineComponent>(pSplineActor->GetComponentByClass(USplineComponent::StaticClass()));

		if (pTargetSpline != nullptr)
		{
			overallLength = pTargetSpline->GetSplineLength();
			//UE_LOG(LogTemp, Warning, TEXT("Spline Found"));
		}
		//else UE_LOG(LogTemp, Warning, TEXT("Spline Not Found "));
	}

}

// Called every frame
void ASecurityPersonel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Patrol();
	//UE_LOG(LogTemp, Warning, TEXT("Caught Actor %d , Alert State %d,anima %d"), caughtActor, Alert, AnimaState);
	//UE_LOG(LogTemp, Warning, TEXT("max : curren %f : %f , temp %f"),overallLength, m_localLength, lenghOnSpline);
}

void ASecurityPersonel::GetCurrenSplinePatrolInfo(FVector& _location, FRotator& _rotation, float _lengh, bool _loop)
{

}

void ASecurityPersonel::Patrol()
{
	
	FVector newLocation;
	FRotator newRotation;
	const FVector& tmpTraceActorPos = DetectionComp->GetTraceActorPosInfo();
	if (Alert)
	{
		AnimaState = AnimationState::RUNNING;
		if (FVector2D(GetActorLocation().X, GetActorLocation().Y).Equals(FVector2D(tmpTraceActorPos.X,tmpTraceActorPos.Y),9.9))
		{
			AnimaState = AnimationState::IDLE;
			if (DetectionComp->IsTraceActorInView())
			{
				caughtActor = true;
			}
			else
			{
				caughtActor = false;
				Alert = false;
			}
		}
		MoveToward(newLocation, newRotation, tmpTraceActorPos, defaultSpeed * 1.5);
	}
	else
	{
		FVector tmpDirectionCheck = pTargetSpline->GetLocationAtDistanceAlongSpline(m_localLength + defaultSpeed, ESplineCoordinateSpace::World)
			- pTargetSpline->GetLocationAtDistanceAlongSpline(m_localLength, ESplineCoordinateSpace::World);

		//UE_LOG(LogTemp, Warning, TEXT("-------------------------------------"));

		//UE_LOG(LogTemp, Warning, TEXT("2 vector angle %f ,spline direction %s, forward vec %s"),
			//tmpDirectionCheck.CosineAngle2D(GetActorRightVector()), *tmpDirectionCheck.ToString(), *GetActorRightVector().ToString());


		/*UE_LOG(LogTemp, Warning, TEXT("onSplineLoc %s , actor loc %s")
			, *pTargetSpline->GetLocationAtDistanceAlongSpline(m_localLength, ESplineCoordinateSpace::World).ToString()
			, *GetActorLocation().ToString(), *tmpDirectionCheck.ToString(), *GetActorRightVector().ToString());*/

		//UE_LOG(LogTemp, Warning, TEXT("---------------------------------"));

		if (CompareActorPos(pTargetSpline->GetLocationAtDistanceAlongSpline(m_localLength, ESplineCoordinateSpace::World)))
		{
			lenghOnSpline += defaultSpeed;
			if (!bLoop) { m_localLength = lenghOnSpline; }
			else
			{
				if (lenghOnSpline >= overallLength)
				{
					if (pTargetSpline->IsClosedLoop() == false)
						uTurn = (uTurn) ? false : true;

					lenghOnSpline = 0;
				}

				m_localLength = FMath::Lerp(0.0f, overallLength, (uTurn) ? (1 - (lenghOnSpline / overallLength)) : (lenghOnSpline / overallLength));
			}
		}
		AnimaState = AnimationState::WALKING;
		MoveToward(newLocation, newRotation, pTargetSpline->GetLocationAtDistanceAlongSpline(m_localLength,ESplineCoordinateSpace::World), defaultSpeed);
	}

	//set location and rotation
	SetActorLocationAndRotation(newLocation, newRotation);
}

void ASecurityPersonel::SetAlertState(bool AlertState)
{
	Alert = AlertState;
}

void ASecurityPersonel::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
	, FVector NormalImpulse, const FHitResult& Hit)
{

}

void ASecurityPersonel::MoveToward(FVector& newLocation, FRotator& newRotation,FVector towardLocation, float const speed)
{
	FVector DirectionVector = towardLocation - GetActorLocation();
	FRotator tmpThisActorRotation = GetActorRotation();

	newRotation = FRotator(tmpThisActorRotation.Pitch, tmpThisActorRotation.Yaw - (DirectionVector.GetSafeNormal(1).CosineAngle2D(GetActorForwardVector()) * turnRate / PI), tmpThisActorRotation.Roll);
	newLocation = GetActorLocation();

	if (DirectionVector.GetSafeNormal(1).CosineAngle2D(GetActorRightVector()) >=0.9)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Coincident"));
		//UE_LOG(LogTemp, Warning, TEXT("current loc %s"), *newLocation.ToString());
		newLocation += DirectionVector.GetSafeNormal(1) * speed;
	}
}

int ASecurityPersonel::GetAnimaState() const
{
	return AnimaState;
}

bool ASecurityPersonel::CompareActorPos(const FVector& v, float errorThreshold)
{
	FVector Actorpos = GetActorLocation();
	if (v.X - errorThreshold <= Actorpos.X && Actorpos.X <= v.X + errorThreshold)
	{
		if (v.Y - errorThreshold <= Actorpos.Y && Actorpos.Y <= v.Y + errorThreshold)
		{
			if (v.Z - errorThreshold <= Actorpos.Z && Actorpos.Z <= v.Z + errorThreshold)
			{
				return true;
			}
		}
	}
	return false;
}