#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"
#include "BTTask_RotateToFace.generated.h"

class AAIController;

UCLASS(config = Game)
class ESPIONAGE_API UBTTask_RotateToFace : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_RotateToFace(const FObjectInitializer& ObjectInitializer);

protected:
	/** Success condition precision in degrees */
	UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0"))
		float Precision;
	UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0",ClampMax = "360.0"))
		float RotationRate;


private:
	/** cached Precision tangent value */
	float PrecisionDot;
	float deltaTimeInSecons;
	FRotator* BeforeRotate;
	FVector* AngleVector;

	bool ValueObserver(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


public:

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);
	virtual FString GetStaticDescription() const override;

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTFocusMemory); }

protected:

	float GetPrecisionDot() const { return PrecisionDot; }
};
