//-------------------------------------------------------------------
//�N���X���FSensa2.cpp
//�T�v	�@�F�Ď��J�����p�����@�i�㉺�ɓ����ASensa1�ƌ��݂ɓ����j
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2021/01/25
//�X�V�@�@�F2021/01/25�@�N���X�쐬
//�X�V�@�@�F2021/01/26  �N���X�C��
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h
#include "Sensa2.h"

//�R���X�g���N�^
ASensa2::ASensa2()
{
 	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ��ł���B
	PrimaryActorTick.bCanEverTick = true;

}

//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void ASensa2::BeginPlay()
{
	Super::BeginPlay();
	//������
	runtime = 0;
}

//���t���[���X�V����
void ASensa2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//���W���擾����
	FVector vNewActorLocation = GetActorLocation();
	//�J��Ԃ��^��
	float move = (FMath::Sin(runtime + DeltaTime) - FMath::Sin(runtime));
	//X���ɉ^���̑����̐ݒ�
	vNewActorLocation.Z -= move * 80;
	//X���ɍ��E�̍ő�l�ɂ���Ĉړ������鏈��
	runtime -= DeltaTime;
	//���W���Ăяo��
	SetActorLocation(vNewActorLocation);
}

