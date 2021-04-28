//-------------------------------------------------------------------
//クラス名：HideObjectBase.cpp
//概要	　：隠れる用アイテムのクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/27
//更新　　：2020/11/27　クラス作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルード
#include "HideObjectBase.h"
#include "Components/BoxComponent.h"
#include"Components/SkeletalMeshComponent.h"
#include"Components/StaticMeshComponent.h"
#include"Engine/StaticMesh.h"
#include"Kismet/GameplayStatics.h"
#include"GameFramework//Character.h"

//コンストラクタ
AHideObjectBase::AHideObjectBase()
	:m_pBoxComp(NULL)
	,m_pStaticMeshComp(NULL)
	,m_pBase(NULL)
	,m_pStaticMesh(NULL)
	,m_pPlayerChara(NULL)
	,m_MeshSize(FVector::ZeroVector)
	,m_isHide(false)
{
 	//毎フレーム、このクラスのTICK()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることができる。
	PrimaryActorTick.bCanEverTick = true;

	//ベースオブジェクト作成
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("HideObjectBase"));
	RootComponent = m_pBase;
	//コリジョン判定用ボックスコンポーネント生成
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	m_pBoxComp->SetupAttachment(RootComponent);

	//スタティックメッシュコンポーネント生成
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	m_pStaticMeshComp->SetupAttachment(RootComponent);

}

//毎フレーム更新処理
void AHideObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_pStaticMesh != NULL)
	{
		//メッシュの中身を確認
		m_pStaticMesh = m_pStaticMeshComp->GetStaticMesh();
	}

	//メッシュの中身が無効であれば進むことができない
	if (m_pStaticMesh != NULL)
	{
		//メッシュのバウンディングボックスサイズを決める
		m_MeshSize = m_pStaticMesh->GetBounds().GetBox().GetSize();

		//オブジェクトのActorのサイズを加味する
		m_MeshSize *= GetActorScale();
	}

	//Tick（）処理の実行状態を無効にしておく
	PrimaryActorTick.SetTickFunctionEnable(false);
}

//毎フレーム更新処理
void AHideObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//隠れる用オブジェクト共通の処理
	HideObject(DeltaTime);
}

//隠れる用オブジェクト共通の処理
void AHideObjectBase::HideObject(float _deltaTime)
{
	//プレイヤーのポインタを、最初の一回だけ取得
	if (m_pPlayerChara == NULL)
	{
		//ゲーム全体に対するActorの検索処理はコストが高いため、BiginPlayでいったん保存するだけにする。
		//検索対象はすべてのActor
		TSubclassOf<AActor>findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*>actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);


		//結果、Actorがあれば
		if (actors.Num() > 0)
		{
			//Actorの中を順番に計算
			for (int idx = 0; idx < actors.Num(); ++idx)
			{
				AActor* pActor = Cast<AActor>(actors[idx]);

				//タグ名で判別する
				if (pActor->ActorHasTag("Player"))
				{
					m_pPlayerChara = pActor;
					break;
				}
			}
		}
	}
	if (m_pPlayerChara != NULL)
	{
		//プレイヤーのメッシュ取得
		//ACharacterクラスの基本クラス（ACharacter）もメッシュを持っているため、
		//ACharacter型にキャストしてGetMesh（）を呼ぶことができる。
		USkeletalMeshComponent* pPlayerMesh = Cast<ACharacter>(m_pPlayerChara)->GetMesh();

		//プレイヤーの角度、座標取得
		FVector playerCharacterLocation = pPlayerMesh->GetComponentLocation();
		FRotator playerCharaRotaion = pPlayerMesh->GetComponentRotation();
		
		if (m_isHide == true)
		{
			//メッシュを非表示
			pPlayerMesh->SetVisibility(false);
			//当たり判定を有効化
			m_pPlayerChara->SetActorEnableCollision(false);
		}
		//隠れていないとき、オブジェクトのTick（）処理の実行状態を無効にする
		else
		{
			//メッシュを表示
			pPlayerMesh->SetVisibility(true);
			//当たり判定を有効化
			m_pPlayerChara->SetActorEnableCollision(true);
			//隠れなくなったので、Tickの（）処理の状態を無効化する
			PrimaryActorTick.SetTickFunctionEnable(false);
		}
	}
}

//隠れているか状態の設定
void AHideObjectBase::SetHide(bool _isHide)
{
	m_isHide = _isHide;

	//隠れるとき、オブジェクトのTick（）処理の状態を有効にする。
	if (m_isHide == true)
	{
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}

