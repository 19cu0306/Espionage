//-------------------------------------------------------------------
//クラス名：GetKey.h
//概要	　：鍵を取ったかどうかのクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/25
//更新　　：2020/11/25　クラス作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルードカード
#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GetKey.generated.h"

UCLASS()
class ESPIONAGE_API AGetKey : public AItemBase
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AGetKey();

public:
	//毎フレーム更新処理
	virtual void Tick(float DeltaTime)override;

private:
	//オーバーラップ接触時
	//派生クラスなのでUFUNCTIONをつけない
	virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	//鍵を入手したかどうかの判定フラグ
	bool m_isGetKey01;
	bool m_isGetKey02;
};
