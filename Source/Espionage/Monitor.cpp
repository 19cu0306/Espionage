//-------------------------------------------------------------------
//�N���X���FMonitor.cpp
//�T�v	�@�F���j�^�[���[���̃��j�^�[�p�̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2021/01/25
//�X�V�@�@�F2021/01/25�@�N���X�쐬
//�X�V�@�@�F2021/01/26  �N���X�C��
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "Monitor.h"
#include"EspionageCharacter.h"
#include"Components/StaticMeshComponent.h"
#include"Components/BoxComponent.h"

//�R���X�g���N�^
AMonitor::AMonitor()
	:m_MonitorCheck(false)
{

}
//�I�[�o�[���b�v�ڐG��
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
                //�t���O��ς���
                m_MonitorCheck = true;

                //�t���O�𔽉f
                pPlayer->MonitorKeyUnlock(m_MonitorCheck);

                //���O�\��
                //UE_LOG(LogTemp, Warning, TEXT("PLAYER UNLOCKED MONITOR"));
            }
        }
    }
}