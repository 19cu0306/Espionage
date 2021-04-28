//19CU0306 - VU NGUYEN ANH  TAI
//UE AI BEHAVIOR TREE
//-----------------------------------


#include "BTTask_Patrol.h"
#include "SecurityCharacter.h"
#include "AI_SecurityCharacter.h"
#include "Components/SplineComponent.h"

// AI Module
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
//key types
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "NavigationSystem.h"


EBTNodeResult::Type ESPIONAGE_API UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_SecurityCharacter* pSCController = Cast<AAI_SecurityCharacter>(OwnerComp.GetAIOwner());
	if (pSCController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ASecurityCharacter* pCharacter = Cast<ASecurityCharacter>(pSCController->GetPawn());

	if (pCharacter->PatrolSpline == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//パトロールSpline
	USplineComponent* pPatrolSpline = Cast<USplineComponent>(pCharacter->PatrolSpline->GetComponentByClass(USplineComponent::StaticClass()));

	if (pPatrolSpline != nullptr)
	{
		//現在の位置とSplineポイントの位置に比べ
		int32 towardSplineIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt("towardSplinePointIndex");
		towardSplineIndex = FMath::RoundToInt(towardSplineIndex);

		if (towardSplineIndex >= pPatrolSpline->GetNumberOfSplinePoints()-1)
		{
			if (pPatrolSpline->IsClosedLoop()) { towardSplineIndex = 0;}
			else if (pCharacter->bPatrolLoop) { pCharacter->bUTurn = true; }
		}

		//Spline　Point　が無い　なら　ブール　ｂUTurnー＞False　（set uturn to false when finish 1 patrol loop(back to first spline point 0)
		if (pCharacter->bUTurn && towardSplineIndex <= 0)
		{
			pCharacter->bUTurn = false;
		}

		//次のSplinePoint　判定　determent the next patrol(spline) point location
		FVector SplinePointLocation = pPatrolSpline->GetLocationAtSplineInputKey(towardSplineIndex,ESplineCoordinateSpace::World);

		if (FVector::Dist(pCharacter->GetActorLocation(),SplinePointLocation)<500)
		{
			pCharacter->bUTurn ? --towardSplineIndex : ++towardSplineIndex;
			OwnerComp.GetBlackboardComponent()->SetValueAsInt("towardSplinePointIndex", towardSplineIndex);
		}

		FVector Location = pPatrolSpline->GetLocationAtSplineInputKey(towardSplineIndex, ESplineCoordinateSpace::World);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("towardPatrolLocation", Location);

		return EBTNodeResult::Succeeded;		
	}

	return EBTNodeResult::Failed;
}