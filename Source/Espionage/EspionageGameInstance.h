// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EspionageGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESPIONAGE_API UEspionageGameInstance : public UGameInstance
{
	GENERATED_BODY()

	

public:
	UPROPERTY(BlueprintReadWrite, Category = "Game Trigger")
		bool bIsGlobalAlert;
	UPROPERTY(BlueprintReadWrite, Category = "Game Trigger")
		float fAlertTime;
	UPROPERTY(BlueprintReadWrite, Category = "Game Trigger")
		bool GameOver;
	UPROPERTY(BlueprintReadWrite, Category = "Game Trigger")
		int totalScore;
	UPROPERTY(BlueprintReadWrite, Category = "Game Trigger")
		float UpTime;

};
