//-------------------------------------------------------------------
//クラス名：Sensa1.h
//概要	　：監視カメラ用処理　（上下に動き、Sensa2と交互に動く）
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
#include "Sensa1.generated.h"

UCLASS()
class ESPIONAGE_API ASensa1 : public AActor
{
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	ASensa1();

protected:
	//ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:	
	//毎フレーム更新処理
	virtual void Tick(float DeltaTime) override;
	//移動時間格納用
	float runtime;
};
