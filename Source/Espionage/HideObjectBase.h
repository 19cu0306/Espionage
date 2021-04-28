//-------------------------------------------------------------------
//クラス名：HideObjectBase.h
//概要	　：隠れる用アイテムのクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/27
//更新　　：2020/11/27　クラス作成
//-------------------------------------------------------------------

//インクルードカード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HideObjectBase.generated.h"


class UBoxComponent;
class USkeltalComponent;
class UStaticMeshComponent;
class UStaticMesh;

UCLASS()
class ESPIONAGE_API AHideObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	AHideObjectBase();

protected:
	//ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:	
	//毎フレーム更新処理
	virtual void Tick(float DeltaTime) override;


private:
	void HideObject(float _deltaTime);

public:
	void SetHide(bool _isLift);

	bool GetHide()const { return m_isHide; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pStaticMeshComp;
	UPROPERTY(EditAnywhere, Category = "Player Character")
		AActor* m_pPlayerChara;

protected:
	UPROPERTY()
		USceneComponent* m_pBase;

private:
	UStaticMesh* m_pStaticMesh;
	
	FVector m_MeshSize;

	bool m_isHide;

};
