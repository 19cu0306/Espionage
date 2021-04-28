//-------------------------------------------------------------------
//クラス名：MoveActorY.cpp
//概要	　：監視カメラ用処理（Y軸用）
//作成者　：19CU0228 野田八雲
//作成日　：2021/01/25
//更新　　：2021/01/25　クラス作成
//更新　　：2021/01/26  クラス修正
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルード
#include "MoveActorY.h"

//コンストラクタ
AMoveActorY::AMoveActorY()
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

}

//ゲームスタート時、または生成時に呼ばれる処理
void AMoveActorY::BeginPlay()
{
	Super::BeginPlay();
	//初期化
	runtime = 0;
}


//毎フレーム更新処理
void AMoveActorY::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector vNewActorLocation = GetActorLocation();
	//繰り返し運動
	float move = (FMath::Sin(runtime + DeltaTime * 1.7) - FMath::Sin(runtime));
	//Y軸に運動の速さの設定
	vNewActorLocation.Y += move * 100;
	//Y軸に左右の最大値によって移動させる処理
	runtime -= DeltaTime;
	//座標を呼び出し
	SetActorLocation(FVector(vNewActorLocation));

}

