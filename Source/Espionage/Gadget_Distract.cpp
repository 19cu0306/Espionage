// Fill out your copyright notice in the Description page of Project Settings.


#include "Gadget_Distract.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGadget_Distract::AGadget_Distract()
	:Loudness(1.0)
	,Range(0.0)
	,NoiseTag(NAME_None)
	,autoDestroy(false)
	,isDisable(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	GadgetMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = BoxComp;
	GadgetMesh->SetupAttachment(RootComponent);
	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("NoiseEmitter");


}

// Called when the game starts or when spawned
void AGadget_Distract::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGadget_Distract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	internalDeltaTime += DeltaTime;

}

void AGadget_Distract::EmitNoise(float Interval,int inCount = 0)
{
	if (isDisable==false)
	{
		if (inCount == 0)		//Infinite Continuously Emit Noise
		{
			if (internalDeltaTime > Interval)
			{
				MakeNoise(Loudness, Cast<APawn>(this), GetActorLocation(), Range, NoiseTag);
			}
		}
		else
		if (internalNoiseCount < inCount)
		{
			if (internalDeltaTime > Interval)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Distract Gadget Emit Noise %d"), internalNoiseCount);
				++internalNoiseCount;
				MakeNoise(Loudness, Cast<APawn>(this), GetActorLocation(), Range, NoiseTag);
				internalDeltaTime = 0;
			}
		}
	}

	if (autoDestroy && inCount != 0 && inCount <= internalNoiseCount)
	{
		Destroy();
	}
	else if (autoDestroy && inCount == 0) Destroy();

}             

bool AGadget_Distract::DisableGadgetSwitch()
{
	if (isDisable)
	{
		isDisable = false;
		SetActorTickEnabled(true);
	}
	else
	{
		isDisable = true;
		internalDeltaTime = 0;
		SetActorTickEnabled(false);
	}
	return isDisable;
}
