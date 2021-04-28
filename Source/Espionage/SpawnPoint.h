//-------------------------------------------------------------------
//クラス名：SpawnPoint.h
//概要	　：プレイヤー初期位置（スタート地点）の処理
//作成者　：19CU0306 ヴー　グエン　アイン　タイ
//作成日　：2020/11/16
//更新　　：2020/11/16　クラス作成
//		　：2020/01/05  プログラム修正
//-------------------------------------------------------------------

//インクルードカード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "SpawnPoint.generated.h"

//前方宣言
class UBoxComponent;

UCLASS()
class ESPIONAGE_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクタ
	ASpawnPoint();

protected:
	//ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;
public:	
	//毎フレーム更新処理
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "SpawnPoint")
		bool DefaultSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "SubSpawnPoints")
		TArray<ASpawnPoint*> SpawnPoint;

	UPROPERTY(EditAnywhere, Category = "CharacterProb")
		ACharacter* Character;

private:
	//宣言
	bool MasterSpawnController;
	FVector getSpawnPos(unsigned int num =0);
	void setSpawn(unsigned int num =0);
	unsigned int SpawnPointNum;
	UBoxComponent* pBoxCollider;			//editor indicator

};
