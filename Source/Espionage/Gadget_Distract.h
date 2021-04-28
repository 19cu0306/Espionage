// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gadget_Distract.generated.h"

UCLASS()
class ESPIONAGE_API AGadget_Distract : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGadget_Distract();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Gadget Property")
		class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category = "Gadget Property")
		UStaticMeshComponent* GadgetMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Gadget Property")
		class UPawnNoiseEmitterComponent* NoiseEmitterComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Gadget Property")
		float Loudness;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Gadget Property")
		float Range;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Gadget Property")
		FName NoiseTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gadget Property")
		bool autoDestroy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gadget Property")
		bool isDisable;

	UFUNCTION(BlueprintCallable, Category = "GadgetFunc")
		void EmitNoise(float Interval, int inCount);
	UFUNCTION(BlueprintCallable, Category = "GadgetFunc")
		bool DisableGadgetSwitch();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float internalDeltaTime;
	int internalNoiseCount = 0;


};
