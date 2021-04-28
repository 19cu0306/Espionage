//-------------------------------------------------------------------
//�N���X���FItemScoreUp.h
//�T�v	�@�F�X�R�A�A�C�e���̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�N���X�쐬
//		 �F2020/12/08  �A�C�e���J�E���g�����쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemScoreUp.generated.h"

/**
 * 
 */
UCLASS()
class ESPIONAGE_API AItemScoreUp : public AItemBase
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	AItemScoreUp();
public:
	//���t���[���X�V����
	virtual void Tick(float DeltaTime)override;

private:
	//�I�[�o�[���b�v�ڐG��
	//�h���N���X�Ȃ̂�UFUNCTION�����Ȃ�
	virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	//�A�C�e���̃X�R�A��������
	UPROPERTY(EditAnyWhere)
		int m_itemAddScore;

	//�A�C�e���J�E���g����
	UPROPERTY(EditAnyWhere)
		int m_itemCnt;

};
