//-------------------------------------------------------------------
//クラス名：Sensa1.cpp
//概要	　：監視カメラ用処理　（上下に動き、Sensa2と交互に動く）
//作成者　：19CU0228 野田八雲
//作成日　：2021/01/25
//更新　　：2021/01/25　クラス作成
//更新　　：2021/01/26  クラス修正
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルード
#include "Sensa1.h"

//コンストラクタ
ASensa1::ASensa1()
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

}

//ゲームスタート時、または生成時に呼ばれる処理
void ASensa1::BeginPlay()
{
	Super::BeginPlay();
	//初期化
	runtime = 0;
}

//毎フレーム更新処理
void ASensa1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//座標を取得する
	FVector vNewActorLocation = GetActorLocation();
	//繰り返し運動
	float move = (FMath::Sin(runtime + DeltaTime) - FMath::Sin(runtime));
	//Z軸に運動の速さの設定
	vNewActorLocation.Z += move * 80;
	//Z軸に左右の最大値によって移動させる処理
	runtime -= DeltaTime;
	//座標を呼び出し
	SetActorLocation(vNewActorLocation);
}

