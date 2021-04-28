//-------------------------------------------------------------------
//�N���X���FHideObjectBase.cpp
//�T�v	�@�F�B���p�A�C�e���̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/27
//�X�V�@�@�F2020/11/27�@�N���X�쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "HideObjectBase.h"
#include "Components/BoxComponent.h"
#include"Components/SkeletalMeshComponent.h"
#include"Components/StaticMeshComponent.h"
#include"Engine/StaticMesh.h"
#include"Kismet/GameplayStatics.h"
#include"GameFramework//Character.h"

//�R���X�g���N�^
AHideObjectBase::AHideObjectBase()
	:m_pBoxComp(NULL)
	,m_pStaticMeshComp(NULL)
	,m_pBase(NULL)
	,m_pStaticMesh(NULL)
	,m_pPlayerChara(NULL)
	,m_MeshSize(FVector::ZeroVector)
	,m_isHide(false)
{
 	//���t���[���A���̃N���X��TICK()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ��ł���B
	PrimaryActorTick.bCanEverTick = true;

	//�x�[�X�I�u�W�F�N�g�쐬
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("HideObjectBase"));
	RootComponent = m_pBase;
	//�R���W��������p�{�b�N�X�R���|�[�l���g����
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	m_pBoxComp->SetupAttachment(RootComponent);

	//�X�^�e�B�b�N���b�V���R���|�[�l���g����
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	m_pStaticMeshComp->SetupAttachment(RootComponent);

}

//���t���[���X�V����
void AHideObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_pStaticMesh != NULL)
	{
		//���b�V���̒��g���m�F
		m_pStaticMesh = m_pStaticMeshComp->GetStaticMesh();
	}

	//���b�V���̒��g�������ł���ΐi�ނ��Ƃ��ł��Ȃ�
	if (m_pStaticMesh != NULL)
	{
		//���b�V���̃o�E���f�B���O�{�b�N�X�T�C�Y�����߂�
		m_MeshSize = m_pStaticMesh->GetBounds().GetBox().GetSize();

		//�I�u�W�F�N�g��Actor�̃T�C�Y����������
		m_MeshSize *= GetActorScale();
	}

	//Tick�i�j�����̎��s��Ԃ𖳌��ɂ��Ă���
	PrimaryActorTick.SetTickFunctionEnable(false);
}

//���t���[���X�V����
void AHideObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�B���p�I�u�W�F�N�g���ʂ̏���
	HideObject(DeltaTime);
}

//�B���p�I�u�W�F�N�g���ʂ̏���
void AHideObjectBase::HideObject(float _deltaTime)
{
	//�v���C���[�̃|�C���^���A�ŏ��̈�񂾂��擾
	if (m_pPlayerChara == NULL)
	{
		//�Q�[���S�̂ɑ΂���Actor�̌��������̓R�X�g���������߁ABiginPlay�ł�������ۑ����邾���ɂ���B
		//�����Ώۂ͂��ׂĂ�Actor
		TSubclassOf<AActor>findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*>actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);


		//���ʁAActor�������
		if (actors.Num() > 0)
		{
			//Actor�̒������ԂɌv�Z
			for (int idx = 0; idx < actors.Num(); ++idx)
			{
				AActor* pActor = Cast<AActor>(actors[idx]);

				//�^�O���Ŕ��ʂ���
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
		//�v���C���[�̃��b�V���擾
		//ACharacter�N���X�̊�{�N���X�iACharacter�j�����b�V���������Ă��邽�߁A
		//ACharacter�^�ɃL���X�g����GetMesh�i�j���ĂԂ��Ƃ��ł���B
		USkeletalMeshComponent* pPlayerMesh = Cast<ACharacter>(m_pPlayerChara)->GetMesh();

		//�v���C���[�̊p�x�A���W�擾
		FVector playerCharacterLocation = pPlayerMesh->GetComponentLocation();
		FRotator playerCharaRotaion = pPlayerMesh->GetComponentRotation();
		
		if (m_isHide == true)
		{
			//���b�V�����\��
			pPlayerMesh->SetVisibility(false);
			//�����蔻���L����
			m_pPlayerChara->SetActorEnableCollision(false);
		}
		//�B��Ă��Ȃ��Ƃ��A�I�u�W�F�N�g��Tick�i�j�����̎��s��Ԃ𖳌��ɂ���
		else
		{
			//���b�V����\��
			pPlayerMesh->SetVisibility(true);
			//�����蔻���L����
			m_pPlayerChara->SetActorEnableCollision(true);
			//�B��Ȃ��Ȃ����̂ŁATick�́i�j�����̏�Ԃ𖳌�������
			PrimaryActorTick.SetTickFunctionEnable(false);
		}
	}
}

//�B��Ă��邩��Ԃ̐ݒ�
void AHideObjectBase::SetHide(bool _isHide)
{
	m_isHide = _isHide;

	//�B���Ƃ��A�I�u�W�F�N�g��Tick�i�j�����̏�Ԃ�L���ɂ���B
	if (m_isHide == true)
	{
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}

