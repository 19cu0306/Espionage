#include "BTTask_RotateToFace.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_RotateToFace::UBTTask_RotateToFace(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Precision(10.f)
{
	//UEに表示する、設定
	NodeName = "Force Rotate to face";
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	//アクタとヴェクトルだけ受け取りに設定する accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToFace, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToFace, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToFace, BlackboardKey));
}

//BehaviorTree　初期化
void UBTTask_RotateToFace::PostInitProperties()
{
	Super::PostInitProperties();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}


void UBTTask_RotateToFace::PostLoad()
{
	Super::PostLoad();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

namespace
{
	FORCEINLINE_DEBUGGABLE float CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
	{
		return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
			? 1.f
			: VectorA.CosineAngle2D(VectorB);
	}
}

//BehaviorTree　実行関数
EBTNodeResult::Type UBTTask_RotateToFace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	const FVector PawnLocation = Pawn->GetActorLocation();
	const FRotator PawnRotation = Pawn->GetActorRotation();
	AngleVector = new FVector;
	BeforeRotate = new FRotator(PawnRotation);
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* ActorValue = Cast<AActor>(KeyValue);
		*AngleVector = (ActorValue->GetActorLocation() - PawnLocation);
		if (ActorValue != NULL)
		{
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector(),*AngleVector);

			if (AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{	
				Result = EBTNodeResult::InProgress;
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		if (FAISystem::IsValidLocation(KeyValue))
		{
			*AngleVector = (KeyValue - PawnLocation);
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector()
				, (KeyValue - PawnLocation));

			if (AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				Result = EBTNodeResult::InProgress;
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		const FRotator KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());

		if (FAISystem::IsValidRotation(KeyValue))
		{
			const FVector DirectionVector = KeyValue.Vector();
			const float AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector(), DirectionVector);
			*AngleVector = DirectionVector;

			if (AngleDifference >= PrecisionDot)
			{
				Result = EBTNodeResult::Succeeded;
			}
			else
			{
				Result = EBTNodeResult::InProgress;
			}
		}
	}

	return Result;
}

//Tick関数
void UBTTask_RotateToFace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		deltaTimeInSecons += DeltaSeconds;
		if (ValueObserver(OwnerComp, NodeMemory))
		{

			const FVector PawnDirection = AIController->GetPawn()->GetActorForwardVector();

			//scuff af need fix proper interpolation
			AIController->GetPawn()->SetActorRotation(FMath::RInterpTo(AIController->GetPawn()->GetActorRotation(), (*AngleVector).Rotation(), DeltaSeconds, RotationRate));

			if (CalculateAngleDifferenceDot(PawnDirection, *AngleVector) >= PrecisionDot)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		else FinishLatentAbort(OwnerComp);

	}
}

//Behavior　
EBTNodeResult::Type UBTTask_RotateToFace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}

FString UBTTask_RotateToFace::GetStaticDescription() const
{
	FString KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc);
}

void UBTTask_RotateToFace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (TaskResult == EBTNodeResult::Succeeded)
	{
		delete BeforeRotate;
		delete AngleVector;
		OwnerComp.GetBlackboardComponent()->ClearValue(FName("NoiseLocation"));
	}
}

bool UBTTask_RotateToFace::ValueObserver(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory)
{
	FVector ObservingValue;
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* ActorValue = Cast<AActor>(KeyValue);

		if (FAISystem::IsValidLocation(ActorValue->GetActorLocation()))
		{
			ObservingValue = (ActorValue->GetActorLocation() - OwnerComp.GetOwner()->GetActorLocation());
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		if (FAISystem::IsValidLocation(KeyValue))
		{
			ObservingValue = (KeyValue - OwnerComp.GetOwner()->GetActorLocation());
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		const FRotator KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());

		if (FAISystem::IsValidRotation(KeyValue))
		{
			ObservingValue = KeyValue.Vector();
		}
	}

	if (*AngleVector == ObservingValue)
	{
		return false;
	}
	else return true;
}