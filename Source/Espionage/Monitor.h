//-------------------------------------------------------------------
//�N���X���FMonitor.h
//�T�v	�@�F���j�^�[���[���̃��j�^�[�p�̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2021/01/25
//�X�V�@�@�F2021/01/25�@�N���X�쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Monitor.generated.h"

/**
 * 
 */
UCLASS()
class ESPIONAGE_API AMonitor : public AItemBase
{
	GENERATED_BODY()
	
public:
	//�R���X�g���N�^
	AMonitor();

private:
	//�I�[�o�[���b�v�ڐG��
	//�h���N���X�Ȃ̂�UFUNCTION�����Ȃ�
	virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	//���j�^�[��t�������ǂ����̔��f
	bool m_MonitorCheck;
};
