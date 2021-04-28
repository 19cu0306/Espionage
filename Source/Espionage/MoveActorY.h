//-------------------------------------------------------------------
//クラス名：MoveActorY.h
//概要	　：監視カメラ用処理（Y軸用）
//作成者　：19CU0228 野田八雲
//作成日　：2021/01/25
//更新　　：2021/01/25　クラス作成
//更新　　：2021/01/26  クラス修正
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルードカード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveActorY.generated.h"

UCLASS()
class ESPIONAGE_API AMoveActorY : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクタ
	AMoveActorY();

protected:
	//ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:	
	//毎フレーム更新処理
	virtual void Tick(float DeltaTime) override;
	//移動時間格納用
	float runtime;

	//プレイヤー情報格納用
	AActor* m_pPlayerChara;

};
