///-------------------------------------------------------------------
//�N���X���FItemScoreUp.cpp
//�T�v	�@�F�X�R�A�A�C�e���̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�N���X�쐬
//		 �F2020/12/08  �A�C�e���J�E���g�����쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "ItemScoreUp.h"
#include"EspionageCharacter.h"
#include"Components/StaticMeshComponent.h"
#include"Components/BoxComponent.h"

//�R���X�g���N�^
AItemScoreUp::AItemScoreUp()
    :m_itemAddScore(0)
    , m_itemCnt(0)
{}

//���t���[���X�V����
void AItemScoreUp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //�X�e�[�g�Ǘ�
    switch (m_state)
    {
        //�����Ȃ�
    case ITEM_STATE_NONE:
    {
        break;
    }

    //�ڐG��
    case ITEM_STATE_COLLIDED:
    {
        m_collidedTimeCnt++;

        if (m_collidedTimeCnt > m_collidedStateTime)
        {
            m_state = ITEM_STATE_DESTOROY;
        }
        break;
    }

    //����
    case ITEM_STATE_DESTOROY:
    {
        this->Destroy();

        m_state = ITEM_STATE_NONE;
        break;
    }

    }

}

//�A�C�e���̃I�[�o�[���b�v����
void AItemScoreUp::OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverRapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    //�ڐG��
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            //UE_LOG(LogTemp, Warning, TEXT("PLAYER GET ITEM"));

            AEspionageCharacter* pPlayer = Cast<AEspionageCharacter>(OtherActor);

            if (pPlayer)
            {
                //�v���C���[�̃X�R�A���ɉ��Z
                pPlayer->GetScore(m_itemAddScore);

                //�A�C�e���̓����蔻�������
                this->SetActorEnableCollision(false);
                //�X�e�[�g����ڐG���ɕύX
                m_state = ITEM_STATE_COLLIDED;

                //���O�\��
                //UE_LOG(LogTemp, Warning, TEXT("PLAYER GET SCORE"),m_itemAddScore);
            }
        }
    }

}

