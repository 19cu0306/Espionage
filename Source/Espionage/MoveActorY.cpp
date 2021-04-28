//-------------------------------------------------------------------
//�N���X���FMoveActorY.cpp
//�T�v	�@�F�Ď��J�����p�����iY���p�j
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2021/01/25
//�X�V�@�@�F2021/01/25�@�N���X�쐬
//�X�V�@�@�F2021/01/26  �N���X�C��
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "MoveActorY.h"

//�R���X�g���N�^
AMoveActorY::AMoveActorY()
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ��ł���B
	PrimaryActorTick.bCanEverTick = true;

}

//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AMoveActorY::BeginPlay()
{
	Super::BeginPlay();
	//������
	runtime = 0;
}


//���t���[���X�V����
void AMoveActorY::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector vNewActorLocation = GetActorLocation();
	//�J��Ԃ��^��
	float move = (FMath::Sin(runtime + DeltaTime * 1.7) - FMath::Sin(runtime));
	//Y���ɉ^���̑����̐ݒ�
	vNewActorLocation.Y += move * 100;
	//Y���ɍ��E�̍ő�l�ɂ���Ĉړ������鏈��
	runtime -= DeltaTime;
	//���W���Ăяo��
	SetActorLocation(FVector(vNewActorLocation));

}

