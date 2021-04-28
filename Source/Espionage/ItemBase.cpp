//-------------------------------------------------------------------
//�N���X���FItemBase.cpp
//�T�v	�@�F�A�C�e���̃x�[�X�N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�x�[�X�N���X�쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "ItemBase.h"
#include"Components/BoxComponent.h"
#include"Components/StaticMeshComponent.h"

//�R���X�g���N�^
AItemBase::AItemBase()
	:m_collidedStateTime(60)
	, m_collidedTimeCnt(0)
	, m_state(ITEM_STATE_NONE)
{
	//���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ��ł���B
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�ɒu���x�[�X�I�u�W�F�N�g����
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = m_pBase;

	//�R���W��������p�{�b�N�X�R���|�[�l���g����
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	m_pBoxComp->SetupAttachment(RootComponent);

	//���b�V���R���|�[�l���g����
	m_pItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	m_pItemMesh->SetupAttachment(RootComponent);
}

//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	//�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
	if (m_pBoxComp != NULL)
	{
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverRapBegin);
	}

}

//���t���[���X�V����
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//�I�[�o�[���b�v����
void AItemBase::OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�h���N���X�ŏ���
}

