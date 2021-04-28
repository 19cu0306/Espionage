//-------------------------------------------------------------------
//�N���X���FGetKey.h
//�T�v	�@�F������������ǂ����̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/25
//�X�V�@�@�F2020/11/25�@�N���X�쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GetKey.generated.h"

UCLASS()
class ESPIONAGE_API AGetKey : public AItemBase
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	AGetKey();

public:
	//���t���[���X�V����
	virtual void Tick(float DeltaTime)override;

private:
	//�I�[�o�[���b�v�ڐG��
	//�h���N���X�Ȃ̂�UFUNCTION�����Ȃ�
	virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	//������肵�����ǂ����̔���t���O
	bool m_isGetKey01;
	bool m_isGetKey02;
};
