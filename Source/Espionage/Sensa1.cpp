//-------------------------------------------------------------------
//�N���X���FSensa1.cpp
//�T�v	�@�F�Ď��J�����p�����@�i�㉺�ɓ����ASensa2�ƌ��݂ɓ����j
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2021/01/25
//�X�V�@�@�F2021/01/25�@�N���X�쐬
//�X�V�@�@�F2021/01/26  �N���X�C��
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "Sensa1.h"

//�R���X�g���N�^
ASensa1::ASensa1()
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ��ł���B
	PrimaryActorTick.bCanEverTick = true;

}

//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void ASensa1::BeginPlay()
{
	Super::BeginPlay();
	//������
	runtime = 0;
}

//���t���[���X�V����
void ASensa1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//���W���擾����
	FVector vNewActorLocation = GetActorLocation();
	//�J��Ԃ��^��
	float move = (FMath::Sin(runtime + DeltaTime) - FMath::Sin(runtime));
	//Z���ɉ^���̑����̐ݒ�
	vNewActorLocation.Z += move * 80;
	//Z���ɍ��E�̍ő�l�ɂ���Ĉړ������鏈��
	runtime -= DeltaTime;
	//���W���Ăяo��
	SetActorLocation(vNewActorLocation);
}

