//-------------------------------------------------------------------
//クラス名：Monitor.cpp
//概要	　：モニタールームのモニター用のクラス
//作成者　：19CU0228 野田八雲
//作成日　：2021/01/25
//更新　　：2021/01/25　クラス作成
//更新　　：2021/01/26  クラス修正
//　　　　：2021/01/26　コメント追加
//-------------------------------------------------------------------

//インクルード
#include "Monitor.h"
#include"EspionageCharacter.h"
#include"Components/StaticMeshComponent.h"
#include"Components/BoxComponent.h"

//コンストラクタ
AMonitor::AMonitor()
	:m_MonitorCheck(false)
{

}
//オーバーラップ接触時
void AMonitor::OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverRapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            //UE_LOG(LogTemp, Warning, TEXT("PLAYER GET ITEM"));

            AEspionageCharacter* pPlayer = Cast<AEspionageCharacter>(OtherActor);

            if (pPlayer)
            {
                //フラグを変える
                m_MonitorCheck = true;

                //フラグを反映
                pPlayer->MonitorKeyUnlock(m_MonitorCheck);

                //ログ表示
                //UE_LOG(LogTemp, Warning, TEXT("PLAYER UNLOCKED MONITOR"));
            }
        }
    }
}