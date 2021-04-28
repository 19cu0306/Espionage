//-------------------------------------------------------------------
//クラス名：ItemScoreUp.h
//概要	　：スコアアイテムのクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/16
//更新　　：2020/11/16　クラス作成
//		 ：2020/12/08  アイテムカウント処理作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルードカード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemScoreUp.generated.h"

/**
 * 
 */
UCLASS()
class ESPIONAGE_API AItemScoreUp : public AItemBase
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AItemScoreUp();
public:
	//毎フレーム更新処理
	virtual void Tick(float DeltaTime)override;

private:
	//オーバーラップ接触時
	//派生クラスなのでUFUNCTIONをつけない
	virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	//アイテムのスコア増減処理
	UPROPERTY(EditAnyWhere)
		int m_itemAddScore;

	//アイテムカウント処理
	UPROPERTY(EditAnyWhere)
		int m_itemCnt;

};
