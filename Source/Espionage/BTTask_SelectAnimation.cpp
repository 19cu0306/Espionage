// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectAnimation.h"
#include "AI_SecurityCharacter.h"
/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type ESPIONAGE_API UBTTask_SelectAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_SecurityCharacter* pSCController = Cast<AAI_SecurityCharacter>(OwnerComp.GetAIOwner());
	if (pSCController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//BTÝ’è‚³‚ê‚½AnimState‚É‚·‚é
	pSCController->AnimationToPlay = SelectAnimation;
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("AnimationState"), SelectAnimation);

	return EBTNodeResult::Succeeded;
}