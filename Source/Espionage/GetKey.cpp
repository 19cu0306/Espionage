//-------------------------------------------------------------------
//�N���X���FGetKey.cpp
//�T�v	�@�F������������ǂ����̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/25
//�X�V�@�@�F2020/11/25�@�N���X�쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "GetKey.h"
#include"ESpionageCharacter.h"
#include"Components/StaticMeshComponent.h"
#include"Components/BoxComponent.h"

//�R���X�g���N�^
AGetKey::AGetKey()
	:m_isGetKey01(false)
    ,m_isGetKey02(false)
{

}
//���t���[���X�V����
void AGetKey::Tick(float DeltaTime)
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
        //�J�E���g
        m_collidedTimeCnt++;
        //�J�E���g�������Ԃ��A�ݒ肵�����Ԃ�蒴������
        if (m_collidedTimeCnt > m_collidedStateTime)
        {
            //�X�e�[�g�����Ŏ��ɐ؂�ւ�
            m_state = ITEM_STATE_DESTOROY;
        }
        break;
    }

    //���Ŏ�
    case ITEM_STATE_DESTOROY:
    {
        //�������g������
        this->Destroy();

        //�X�e�[�g�������Ȃ���Ԃɂ��ǂ�
        m_state = ITEM_STATE_NONE;
        break;
    }

    }
}
//�A�C�e���̃I�[�o�[���b�v����
void AGetKey::OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverRapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    //�ڐG��
	if (OtherComp && (OtherActor != this) && OtherComp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PLAYER GET ITEM"));

        //�v���C���[�����擾
		AEspionageCharacter* pPlayer = Cast<AEspionageCharacter>(OtherActor);

        //���ɏՓ˂������ɂ���ď����𕪊�
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

