// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI_SecurityCharacter.h"
#include "BTTask_SelectAnimation.generated.h"

/**
 * 
 */
UCLASS()
class ESPIONAGE_API UBTTask_SelectAnimation : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(Category = Animation, EditAnywhere)
		TEnumAsByte<AnimationState> SelectAnimation;
};
