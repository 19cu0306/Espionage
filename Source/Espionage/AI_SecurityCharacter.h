//-------------------------------------------------------------------
//クラス名：AI_SecurityCharacter.cpp
//概要	　：AIキャラ（警備員）の処理
//作成者　：19CU0306 ヴー　グエン　アイン　タイ
//-------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AI_SecurityCharacter.generated.h"


UENUM(BlueprintType)
enum AnimationState
{
	IDLE,
	WALKING,
	RUNNING
};
/**
 * 
 */
UCLASS()
class ESPIONAGE_API AAI_SecurityCharacter : public AAIController
{
	GENERATED_BODY()

		AAI_SecurityCharacter();

	/* Called whenever the controller possesses a character bot */
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;
	virtual void Tick(float DeltaTime) override;

	class UBehaviorTreeComponent* BehaviorComp;
	class UBlackboardComponent* BlackboardComp;

public:
	//void setTargetEnemy(APawn* newTarget);
	//void setNoiseLocation(const FVector& NoiseLocation);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecurityCharAnimation")
		TEnumAsByte<AnimationState> AnimationToPlay;



	UPROPERTY(EditAnywhere, Category = "AI Sense")
		class UAIPerceptionComponent* PawnPerceptionComp;
	UPROPERTY(EditAnywhere, Category = "AI Sense")
		float ObserveTime;
	UPROPERTY(BlueprintReadOnly, Category = "AI Sense")
		bool isAlarm;
	UPROPERTY(BlueprintReadOnly, Category = "AI Sense")
		bool isCatchActor;


	UFUNCTION(BlueprintCallable, Category = "AI Perception")
		void OnTargetSenseUpdate(AActor* actor, FAIStimulus stimulus);
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
		void OnPerceptionUpdate(const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
		FGenericTeamId GetTeamId()const { return GetGenericTeamId(); }
	UFUNCTION(BlueprintCallable, Category = "AI Perception")
		bool GetIsSawPlayer()const { return isSawPlayer; }

	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsAlert()const { return isShouldAlert; }
	UFUNCTION(BlueprintCallable, Category = "State")
		float GetEnemyObserveTime()const { return playerObserveDeltaTime; }

	void SetHitByPlayer(bool isHit);

private:
	class UAISenseConfig_Hearing* PawnHearing;
	class UAISenseConfig_Sight* PawnSight;
	

	TArray<AActor*> PerceivingHostileActors;
	APawn* pSensedTarget;
	//sp character touch Player
	bool isShouldAlert;
	bool isHitByPlayer;

	bool isSawPlayer;
	float playerObserveDeltaTime;
	bool seeSomething;
	AActor* sightTrackingActor;

	bool heardSomething;
	float heardSomethingDeltaTime;
	uint8 HeardSomethingCount;
	FAIStimulus hearingStimulus;

	class ASecurityCharacter* SecurityPawn;

	void initPatrolLoc();

	FAISenseID HearingSenseID;
	FAISenseID SightSenseID;
	FAISenseID TouchSenseID;
};