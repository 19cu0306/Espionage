//-------------------------------------------------------------------
//クラス名：SpawnPoint.cpp
//概要	　：プレイヤー初期位置（スタート地点）の処理
//作成者　：19CU0306 ヴー　グエン　アイン　タイ
//作成日　：2020/11/16
//更新　　：2020/11/16　クラス作成
//		　：2020/01/05  プログラム修正
//-------------------------------------------------------------------

//インクルード
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

//コンストラクタ
ASpawnPoint::ASpawnPoint():DefaultSpawnPoint(false)
{
 	//毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = false;

	//コリジョン判定用ボックスコンポーネント生成
	pBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	pBoxCollider->SetupAttachment(RootComponent);
}

//ゲームスタート時、または生成時に呼ばれる処理
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (Character == NULL) { 
		//Character = LoadClass<Aplayer01>(NULL, TEXT("player01"), NULL, LOAD_None, NULL);
		//UE_LOG(LogClass, Warning, TEXT("Actor is NOT SET"));
	}

	SpawnPointNum = SpawnPoint.Num();
	
	if (SpawnPointNum <= 0)
	{
		MasterSpawnController = false;
	}
	else MasterSpawnController = true;



}

//毎フレーム更新処理
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnPoint::setSpawn(unsigned int num)
{
	if(num<SpawnPointNum)
		Character->SetActorLocationAndRotation(SpawnPoint[num]->GetActorLocation(), SpawnPoint[num]->GetActorRotation());
	else 	Character->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());
}

FVector ASpawnPoint::getSpawnPos(unsigned int num)
{
	if (num < SpawnPointNum)
		return SpawnPoint[num]->GetActorLocation();
	else return this->GetActorLocation();
}
