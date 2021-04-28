//-------------------------------------------------------------------
//クラス名：AI_SecurityCharacter.cpp
//概要	　：AIキャラ（警備員）の処理
//作成者　：19CU0306 ヴー　グエン　アイン　タイ
//作成日　：2020/11/16
//更新　　：2020/11/16　クラス作成
//		　：2020/02/11  プログラム修正
//-------------------------------------------------------------------

//インクルード
#include "AI_SecurityCharacter.h"

//Actor Component
#include "EspionageCharacter.h"
#include "SecurityCharacter.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//AI
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Touch.h"
#include "Perception/AISense_Touch.h"
#include "Perception/AIPerceptionTypes.h"
#include "GenericTeamAgentInterface.h"

//GameState -> change to game instance
#include "EspionageGameInstance.h"

//コンストラクタ
AAI_SecurityCharacter::AAI_SecurityCharacter()
	:AnimationToPlay(AnimationState::IDLE)
	, ObserveTime(0)
	, isHitByPlayer(false)
	, playerObserveDeltaTime(0)
	, seeSomething(false)
	, isCatchActor(false)
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	//AI
	PawnPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PawnPerception"));

	PawnHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	PawnHearing->SetMaxAge(3.0f);

	PawnSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	PawnSight->PeripheralVisionAngleDegrees = 75;
	PawnSight->LoseSightRadius = 2000;
	PawnSight->SightRadius = 1800;

	PawnPerceptionComp->ConfigureSense(*PawnHearing);
	PawnPerceptionComp->ConfigureSense(*PawnSight);
	PawnPerceptionComp->SetDominantSense(PawnSight->GetSenseImplementation());
	
	PerceptionComponent = PawnPerceptionComp;
	SetGenericTeamId(1);
}


void AAI_SecurityCharacter::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	HearingSenseID = UAISense::GetSenseID<UAISense_Hearing>();
	SightSenseID = UAISense::GetSenseID<UAISense_Sight>();
	
	SecurityPawn = Cast<ASecurityCharacter>(InPawn);
	if (SecurityPawn)
	{
		SecurityPawn->bUseControllerRotationYaw = true;
	
		if (SecurityPawn->BehaviorTree  && SecurityPawn->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(SecurityPawn->BehaviorTree->BlackboardAsset));
			BlackboardComp->SetValueAsEnum("AnimationState", AnimationState::IDLE);
			BlackboardComp->SetValueAsBool("SeePlayer", false);
			BlackboardComp->SetValueAsBool("HeardPlayer", false);

			initPatrolLoc();
		}
		if (SecurityPawn->BehaviorTree != nullptr)
		{
			BehaviorComp->StartTree(*SecurityPawn->BehaviorTree);
		}
	}

	PawnPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAI_SecurityCharacter::OnPerceptionUpdate);
	PawnPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_SecurityCharacter::OnTargetSenseUpdate);
}


void AAI_SecurityCharacter::OnUnPossess()
{
	Super::OnUnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
	
}

//dont work for some reason
void AAI_SecurityCharacter::Tick(float DeltaTime)
{
	//force rotation to owner pawn
	SetControlRotation(GetPawn()->GetActorRotation());

	for (int i = 0; i < PerceivingHostileActors.Num(); ++i)
	{
		if (PawnPerceptionComp->HasAnyCurrentStimulus(*PerceivingHostileActors[i]))
		{
			continue;
		}
		else if (i == PerceivingHostileActors.Num() - 1)
		{
			isShouldAlert = false;
		}
	}

	if (isShouldAlert)
	{
		BlackboardComp->SetValueAsBool(FName("Patrolling"), false);

		//start suspisous delta time tick
		if (seeSomething)
		{
			playerObserveDeltaTime += DeltaTime;

			if (PerceptionComponent->HasActiveStimulus(*sightTrackingActor, SightSenseID) && playerObserveDeltaTime >= ObserveTime)
			{
				BlackboardComp->SetValueAsBool(FName("GoInvestigateNoise"), false);
				BlackboardComp->SetValueAsBool(FName("seePlayer"), true);
				BlackboardComp->SetValueAsVector(FName("EnemyLocation"), sightTrackingActor->GetActorLocation());
				isSawPlayer = true;
			}
		}
		else playerObserveDeltaTime = 0;

		if (heardSomething)
		{
			heardSomethingDeltaTime += DeltaTime;

			if (HeardSomethingCount >= 3)
			{
				BlackboardComp->SetValueAsBool(FName("GoInvestigateNoise"), true);
			}
			else
			{
				BlackboardComp->SetValueAsBool(FName("GoInvestigateNoise"), false);
			}

			if (heardSomethingDeltaTime >= PerceptionComponent->GetSenseConfig(HearingSenseID)->GetMaxAge())
			{
				HeardSomethingCount = 0;
				heardSomething = false;
				heardSomethingDeltaTime = 0;
			}

		}
		if (isSawPlayer)
		{
			Cast<UEspionageGameInstance>(GetGameInstance())->bIsGlobalAlert = true;
		}
	}
	else BlackboardComp->SetValueAsBool(FName("Patrolling"), true);


	if (isHitByPlayer)
	{
		TArray<AActor*> PerceivingActors;
		PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivingActors);
		for (int i = 0; i < PerceivingActors.Num(); ++i)
		{
			if (PerceivingActors[i]->ActorHasTag("Player"))
			{
				Cast<UEspionageGameInstance>(GetGameInstance())->GameOver = true;
				isCatchActor = true;		
			}
			else isHitByPlayer = false;
		}
	}
}

void AAI_SecurityCharacter::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	initPatrolLoc();
}

void AAI_SecurityCharacter::initPatrolLoc()
{
	if (SecurityPawn)
	{
		if (SecurityPawn->PatrolSpline)
		{
			USplineComponent* pPatrolSpline = Cast<USplineComponent>(SecurityPawn->PatrolSpline->GetComponentByClass(USplineComponent::StaticClass()));
			if (pPatrolSpline)
				BlackboardComp->SetValueAsInt("towardSplinePointIndex", (int)(pPatrolSpline->FindInputKeyClosestToWorldLocation(SecurityPawn->GetActorLocation()) + 1));
		}
	}
}

void AAI_SecurityCharacter::OnPerceptionUpdate(const TArray<AActor*>& Actors)
{
	PawnPerceptionComp->ForgetActor(this);

	for (int i = 0; i < Actors.Num(); ++i)
	{
		if (Actors[i]->ActorHasTag(FName("Player")))
		{
			PerceivingHostileActors.Add(Actors[i]);
		}
		if (Actors[i]->ActorHasTag(FName("PlayerGadget")))
		{

		}
	}

	isShouldAlert = true;
}

void AAI_SecurityCharacter::OnTargetSenseUpdate(AActor* actor, FAIStimulus stimulus)
{
	
	if (Cast<AEspionageCharacter>(actor)->IsHiding())
	{
		PerceptionComponent->ForgetActor(actor);
	}
	else
	if (actor->ActorHasTag("Player") && FVector::Dist(GetPawn()->GetActorLocation(), stimulus.StimulusLocation) < 750.0f)
	{
		BlackboardComp->SetValueAsBool(FName("SeePlayer"), true);
		isSawPlayer = true;
	}

	if (stimulus.Type == HearingSenseID)
	{
		if ( PawnPerceptionComp->HasActiveStimulus(*actor, HearingSenseID))
		{
			heardSomething = true;
			BlackboardComp->SetValueAsBool(FName("heardSomething"), true);
			FVector HeardSomethingLocation = stimulus.StimulusLocation;
			BlackboardComp->SetValueAsVector(FName("NoiseLocation"), HeardSomethingLocation);
		}
		else
		{
			BlackboardComp->SetValueAsBool(FName("heardSomething"), false);
		}

		if (heardSomething && heardSomethingDeltaTime < PawnPerceptionComp->GetSenseConfig(HearingSenseID)->GetMaxAge())
		{
			hearingStimulus = stimulus;
			HeardSomethingCount++;
			UE_LOG(LogTemp, Warning, TEXT("hearing noise in short interval, count %d"), (int)HeardSomethingCount);
		}
	}

	if (stimulus.Type == SightSenseID)
	{
		if (actor != nullptr && PawnPerceptionComp->HasActiveStimulus(*actor, SightSenseID))
		{
			seeSomething = true;
			sightTrackingActor = actor;
			FVector SawSomethingLocation = actor->GetActorLocation();
			BlackboardComp->SetValueAsVector(FName("EnemyLocation"), SawSomethingLocation);
			BlackboardComp->SetValueAsBool(FName("seeSomething"), true);
		}
		else
		{
			seeSomething = false;
			BlackboardComp->SetValueAsBool(FName("seeSomething"), false);
			BlackboardComp->SetValueAsBool(FName("SeePlayer"), false);
			BlackboardComp->ClearValue(FName("EnemyLocation"));
		}
	}
}

void AAI_SecurityCharacter::SetHitByPlayer(bool isHit)
{
	isHitByPlayer = isHit;
}