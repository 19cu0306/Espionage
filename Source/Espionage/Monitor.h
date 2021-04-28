//-------------------------------------------------------------------
//クラス名：Monitor.h
//概要	　：モニタールームのモニター用のクラス
//作成者　：19CU0228 野田八雲
//作成日　：2021/01/25
//更新　　：2021/01/25　クラス作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルードカード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Monitor.generated.h"

/**
 * 
 */
UCLASS()
class ESPIONAGE_API AMonitor : public AItemBase
{
	GENERATED_BODY()
	
public:
	//コンストラクタ
	AMonitor();

private:
	//オーバーラップ接触時
	//派生クラスなのでUFUNCTIONをつけない
	virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	//モニターを付けたかどうかの判断
	bool m_MonitorCheck;
};
