// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spline_Patrol.generated.h"

class USplineComponent;

UCLASS()
class ESPIONAGE_API ASpline_Patrol : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpline_Patrol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USplineComponent* pSplineComp;
};
