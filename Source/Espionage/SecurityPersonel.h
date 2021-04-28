// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SecurityPersonel.generated.h"

class USplineComponent;
class UDetection;

enum AnimationState
{
	IDLE,
	WALKING,
	RUNNING
};
UCLASS()
class ESPIONAGE_API ASecurityPersonel : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASecurityPersonel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Skeletal Mesh")
		USkeletalMeshComponent* pSkeletalMesh;
	UPROPERTY()
		UDetection* DetectionComp;
	UPROPERTY(EditAnywhere, Category = "Spline")
		AActor* pSplineActor;				//direct spline referrence

	UPROPERTY(EditAnywhere, Category = "SecurityActor")
		float defaultSpeed;
	UPROPERTY(EditAnywhere, Category = "SecurityActor")
		bool bLoop;
	UPROPERTY(EditAnywhere, Category = "SecurityActor")
		float turnRate;

	UFUNCTION(BlueprintCallable, Category = "SP Animation")
		int GetAnimaState() const;


	UFUNCTION()
		void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, FVector NormalImpulse, const FHitResult& Hit);

	void GetCurrenSplinePatrolInfo(FVector& _location, FRotator& _rotation, float _lengh = 0.0f, bool _loop = false);
	void SetAlertState(bool AlertState);
private:
	float m_localLength;
	//float deltaTime;
	float lenghOnSpline;
	float overallLength;
	bool uTurn;
	bool Alert;
	bool caughtActor;
	unsigned int AnimaState;

	void Patrol();
	void MoveToward(FVector& newLocation, FRotator& newRotation,FVector towardLocation, float const speed);
	bool CompareActorPos(const FVector& v, float errorThreshold=2.0f);

	USplineComponent* pTargetSpline;				//direct spline referrence

};
