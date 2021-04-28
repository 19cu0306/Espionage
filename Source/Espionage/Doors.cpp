//Project			:Espionage
//ファイル名		:Doors.cpp
// 作成者			:19CU0306	ANH TAI
//概要				:インベントリ
//歴史				:2020/10/20 作成	
//-----------------------------------------------------------------------
//作成者			:19CU0228	野田八雲
//更新履歴			:2021/01/19 ドア処理追加(鍵ごとにドアを開ける)
//  				:2021/01/26 ドア処理追加 （鍵なし用ドア）
//-----------------------------------------------------------------------


//インクルード
#include "Doors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include"EspionageCharacter.h"
#include"GetKey.h"

//コンストラクタ
ADoors::ADoors()
	:pStaticMesh(NULL)
	,pBoxCollider(NULL)
	, bOpen(false)
	, turnTime(1)
	, TargetPos(NULL)
	, TargetAngle(120.f)
	, CloseRot(NULL)
	, radius(0.f)
	, angle(0)
	, deltaTime(0.f)
	, lockState(false)
	,pCharacter(NULL)
{
	//アクターのTick()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Doors");

	pRootComponent= CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = pRootComponent;
	pBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	pBoxCollider->SetupAttachment(RootComponent);
	pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	pStaticMesh->SetupAttachment(RootComponent);
	
	pBoxCollider->SetCollisionProfileName("OverlapOnlyPawn");
}

//ゲーム開始時、または生成時に呼ばれる処理
void ADoors::BeginPlay()
{
	Super::BeginPlay();

	if (pBoxCollider != NULL)
	{
		pBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ADoors::OnOverlapBegin);
		pBoxCollider->OnComponentEndOverlap.AddDynamic(this, &ADoors::OnOverlapEnd);
	}

	SetActorTickEnabled(false);

	if (password != "" || keyObject != nullptr)
	{
		lockState = true;
	}
	else lockState = false;


	CloseRot = pStaticMesh->GetComponentRotation();
	ClosePos = pStaticMesh->GetComponentLocation();
}

//毎フレーム更新処理
void ADoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ドアー開閉からの時間
	deltaTime += DeltaTime;

	//時間制御
	if (deltaTime >= turnTime)
	{
		deltaTime = turnTime;
	}

	//ドアの種類
	switch (DoorType)
	{
	case DOOR_TYPE::SWING:
		TurnDoor();
		break;
	case DOOR_TYPE::SLIDE:
		SlideDoor();
		break;
	}
	/*switch (LockType)
	{
	case DOOR_LOCK_NOLOCK:
		break;
	case DOOR_LOCK_KEY:
		if (lockState == false)
		{
			UnlockDoor();
		}
		break;
	case DOOR_LOCK_PASSWORD:
		break;
	}*/
}

void ADoors::SetState()
{
	if (deltaTime >= turnTime || deltaTime==0)
	{
		deltaTime = 0;
	}
	else { deltaTime = turnTime - deltaTime;  }

	if (lockState)
	{
		bOpen = false;
	}
	else
	{
		bOpen = bOpen ? false : true;
		SetActorTickEnabled(true);
	}

}

//回転ドア
void ADoors::TurnDoor()
{
	angle = FMath().Lerp(CloseRot.Yaw, TargetAngle + CloseRot.Yaw, bOpen ? deltaTime / turnTime : 1 - deltaTime / turnTime);
	float rad = FMath().DegreesToRadians(angle);

	FRotator newRotation = FRotator(CloseRot.Roll, angle, CloseRot.Pitch);

	pStaticMesh->SetWorldLocationAndRotation(ClosePos, newRotation);

	FRotator tmpRot = pStaticMesh->GetComponentRotation();

	if (tmpRot.Yaw >= TargetAngle + CloseRot.Yaw && bOpen || tmpRot.Yaw == 0 && bOpen == false)
	{
		SetActorTickEnabled(false); //comment out this to test or debug
		deltaTime = 0;
	}
}

//スライドドア
void ADoors::SlideDoor()
{
	pStaticMesh->SetWorldLocationAndRotation(FMath().Lerp(ClosePos, TargetPos, bOpen ? deltaTime / turnTime : 1 - deltaTime / turnTime), GetActorRotation());

	FVector tmpPos = pStaticMesh->GetComponentLocation();
	if (tmpPos == TargetPos && bOpen || tmpPos == ClosePos && bOpen == false)
	{
		SetActorTickEnabled(false); //comment out this to test or debug
		deltaTime = 0;
	}
}

//ドアの鍵解除
void ADoors::UnlockDoor(FString _password, AActor* _keyObject)
{
	if (_password == password|| _keyObject == keyObject)
	{
		//UE_LOG(LogTemp, Warning, TEXT("overlap start"));
	}
}

//Actorとオーバーラップした時の処理
void ADoors::OnOverlapBegin(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult)
{
	//オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			//どのドアかによって条件式が変わる
			//UE_LOG(LogTemp, Warning, TEXT("overlap start"));;
			pCharacter = Cast<AEspionageCharacter,AActor>(OtherActor);
			AEspionageCharacter* tmp = Cast<AEspionageCharacter>(OtherActor);
			if (bAuto && this->ActorHasTag("Doors"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
			else if (bAuto && tmp->GetKey01() == true && this->ActorHasTag("Door01"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
			else if (bAuto && tmp->GetKey02() == true && this->ActorHasTag("Door02"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
			else if (bAuto && tmp->MonitorKeyUnlock() == true && this->ActorHasTag("Door03"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
		}
	}
}

//オーバーラップが終了したときの処理
void ADoors::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	//UE_LOG(LogTemp, Warning, TEXT("overlap start"));;
	pCharacter = Cast<AEspionageCharacter, AActor>(OtherActor);
	AEspionageCharacter* tmp = Cast<AEspionageCharacter>(OtherActor);

	//ドアの種類によって条件式が変わる。
	//UE_LOG(LogTemp, Warning, TEXT("overlap end"));
	if (tmp != nullptr)
	{
		if (bAuto)
		{
			//ドア01（主人の部屋）
			if (tmp->GetKey01() == true && this->ActorHasTag("Door01"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
			//ドア02（コントロールルーム）
			else if (tmp->GetKey02() == true && this->ActorHasTag("Door02"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
			//ドア03（コントロールルームによる鍵の解除用）
			else if (tmp->MonitorKeyUnlock() == true && this->ActorHasTag("Door03"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
			//鍵なしドア
			else if (this->ActorHasTag("Doors"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
		}
	}

}