//-------------------------------------------------------------------
//�N���X���FSensa1.h
//�T�v	�@�F�Ď��J�����p�����@�i�㉺�ɓ����ASensa2�ƌ��݂ɓ����j
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2021/01/25
//�X�V�@�@�F2021/01/25�@�N���X�쐬
//�X�V�@�@�F2021/01/26  �N���X�C��
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sensa1.generated.h"

UCLASS()
class ESPIONAGE_API ASensa1 : public AActor
{
	GENERATED_BODY()
	
public:	
	//�R���X�g���N�^
	ASensa1();

protected:
	//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:	
	//���t���[���X�V����
	virtual void Tick(float DeltaTime) override;
	//�ړ����Ԋi�[�p
	float runtime;
};
