//-------------------------------------------------------------------
//クラス名：ItemBase.h
//概要	　：アイテムのベースクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/16
//更新　　：2020/11/16　ベースクラス作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルードカード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

//前方宣言
class UBoxComponent;
class UStaticMeshComponent;


UCLASS()
class ESPIONAGE_API AItemBase : public AActor
{
protected:
	//アイテムステート
	enum ITEM_STATE
	{
		ITEM_STATE_NONE = 0,		//何も起こってない
		ITEM_STATE_COLLIDED,		//接触時
		ITEM_STATE_DESTOROY,		//消滅
		ITEM_STATE_NUM				//ステート数
	};
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	AItemBase();

protected:
	//毎フレーム更新処理
	virtual void BeginPlay() override;

public:	
	//毎フレーム更新処理
	virtual void Tick(float DeltaTime) override;

protected:
	//オーバーラップ接触し始めたときに呼ばれるイベント関数を登録
	UFUNCTION()
		virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//エディタ編集できるメンバ変数
public:
	UPROPERTY(EditAnyWhere, BluePrintReadWrite)
		UBoxComponent* m_pBoxComp;			//アイテム接触用コリジョン取得

	UPROPERTY(EditAnyWhere, BluePrintReadWrite)
		UStaticMeshComponent* m_pItemMesh;	//アイテムメッシュ

protected:
	//ベースコンポーネント
	UPROPERTY()
		USceneComponent* m_pBase;

	//接触演出設定時間
	UPROPERTY()
		int m_collidedStateTime;

	//派生クラスで使えるように設定する変数
protected:
	int m_collidedTimeCnt;		//接触演出カウント
	ITEM_STATE m_state;			//ステート管理
};
