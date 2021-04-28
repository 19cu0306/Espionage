///-------------------------------------------------------------------
//クラス名：ItemScoreUp.cpp
//概要	　：スコアアイテムのクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/16
//更新　　：2020/11/16　クラス作成
//		 ：2020/12/08  アイテムカウント処理作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルード
#include "ItemScoreUp.h"
#include"EspionageCharacter.h"
#include"Components/StaticMeshComponent.h"
#include"Components/BoxComponent.h"

//コンストラクタ
AItemScoreUp::AItemScoreUp()
    :m_itemAddScore(0)
    , m_itemCnt(0)
{}

//毎フレーム更新処理
void AItemScoreUp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //ステート管理
    switch (m_state)
    {
        //何もない
    case ITEM_STATE_NONE:
    {
        break;
    }

    //接触時
    case ITEM_STATE_COLLIDED:
    {
        m_collidedTimeCnt++;

        if (m_collidedTimeCnt > m_collidedStateTime)
        {
            m_state = ITEM_STATE_DESTOROY;
        }
        break;
    }

    //消滅
    case ITEM_STATE_DESTOROY:
    {
        this->Destroy();

        m_state = ITEM_STATE_NONE;
        break;
    }

    }

}

//アイテムのオーバーラップ処理
void AItemScoreUp::OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverRapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    //接触時
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            //UE_LOG(LogTemp, Warning, TEXT("PLAYER GET ITEM"));

            AEspionageCharacter* pPlayer = Cast<AEspionageCharacter>(OtherActor);

            if (pPlayer)
            {
                //プレイヤーのスコア情報に加算
                pPlayer->GetScore(m_itemAddScore);

                //アイテムの当たり判定を消す
                this->SetActorEnableCollision(false);
                //ステート情報を接触時に変更
                m_state = ITEM_STATE_COLLIDED;

                //ログ表示
                //UE_LOG(LogTemp, Warning, TEXT("PLAYER GET SCORE"),m_itemAddScore);
            }
        }
    }

}

