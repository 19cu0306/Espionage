//-------------------------------------------------------------------
//クラス名：GetKey.cpp
//概要	　：鍵を取ったかどうかのクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/25
//更新　　：2020/11/25　クラス作成
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルード
#include "GetKey.h"
#include"ESpionageCharacter.h"
#include"Components/StaticMeshComponent.h"
#include"Components/BoxComponent.h"

//コンストラクタ
AGetKey::AGetKey()
	:m_isGetKey01(false)
    ,m_isGetKey02(false)
{

}
//毎フレーム更新処理
void AGetKey::Tick(float DeltaTime)
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
        //カウント
        m_collidedTimeCnt++;
        //カウントした時間が、設定した時間より超えたら
        if (m_collidedTimeCnt > m_collidedStateTime)
        {
            //ステートを消滅時に切り替え
            m_state = ITEM_STATE_DESTOROY;
        }
        break;
    }

    //消滅時
    case ITEM_STATE_DESTOROY:
    {
        //自分自身を消す
        this->Destroy();

        //ステートを何もない状態にもどす
        m_state = ITEM_STATE_NONE;
        break;
    }

    }
}
//アイテムのオーバーラップ処理
void AGetKey::OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverRapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    //接触時
	if (OtherComp && (OtherActor != this) && OtherComp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PLAYER GET ITEM"));

        //プレイヤー情報を取得
		AEspionageCharacter* pPlayer = Cast<AEspionageCharacter>(OtherActor);

        //何に衝突したかによって処理を分岐
        if (pPlayer && this->ActorHasTag("GetKey01"))
		{
            m_isGetKey01 = true;

			pPlayer->GetKey01(m_isGetKey01);

			this->SetActorEnableCollision(false);

			m_state = ITEM_STATE_COLLIDED;
		}
        if (pPlayer && this->ActorHasTag("GetKey02"))
        {
            m_isGetKey02 = true;

            pPlayer->GetKey02(m_isGetKey02);

            this->SetActorEnableCollision(false);

            m_state = ITEM_STATE_COLLIDED;
        }
	}
}

