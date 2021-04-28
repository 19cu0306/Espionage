//-------------------------------------------------------------------
//クラス名：ItemBase.cpp
//概要	　：アイテムのベースクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/16
//更新　　：2020/11/16　ベースクラス作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルード
#include "ItemBase.h"
#include"Components/BoxComponent.h"
#include"Components/StaticMeshComponent.h"

//コンストラクタ
AItemBase::AItemBase()
	:m_collidedStateTime(60)
	, m_collidedTimeCnt(0)
	, m_state(ITEM_STATE_NONE)
{
	//毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることもできる。
	PrimaryActorTick.bCanEverTick = true;

	//ルートに置くベースオブジェクト生成
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = m_pBase;

	//コリジョン判定用ボックスコンポーネント生成
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	m_pBoxComp->SetupAttachment(RootComponent);

	//メッシュコンポーネント生成
	m_pItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	m_pItemMesh->SetupAttachment(RootComponent);
}

//ゲームスタート時、または生成時に呼ばれる処理
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
	if (m_pBoxComp != NULL)
	{
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverRapBegin);
	}

}

//毎フレーム更新処理
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//オーバーラップ処理
void AItemBase::OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//派生クラスで処理
}

