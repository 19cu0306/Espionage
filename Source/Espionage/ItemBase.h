//-------------------------------------------------------------------
//�N���X���FItemBase.h
//�T�v	�@�F�A�C�e���̃x�[�X�N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�x�[�X�N���X�쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

//�O���錾
class UBoxComponent;
class UStaticMeshComponent;


UCLASS()
class ESPIONAGE_API AItemBase : public AActor
{
protected:
	//�A�C�e���X�e�[�g
	enum ITEM_STATE
	{
		ITEM_STATE_NONE = 0,		//�����N�����ĂȂ�
		ITEM_STATE_COLLIDED,		//�ڐG��
		ITEM_STATE_DESTOROY,		//����
		ITEM_STATE_NUM				//�X�e�[�g��
	};
	GENERATED_BODY()
	
public:	
	//�R���X�g���N�^
	AItemBase();

protected:
	//���t���[���X�V����
	virtual void BeginPlay() override;

public:	
	//���t���[���X�V����
	virtual void Tick(float DeltaTime) override;

protected:
	//�I�[�o�[���b�v�ڐG���n�߂��Ƃ��ɌĂ΂��C�x���g�֐���o�^
	UFUNCTION()
		virtual void OnOverRapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�G�f�B�^�ҏW�ł��郁���o�ϐ�
public:
	UPROPERTY(EditAnyWhere, BluePrintReadWrite)
		UBoxComponent* m_pBoxComp;			//�A�C�e���ڐG�p�R���W�����擾

	UPROPERTY(EditAnyWhere, BluePrintReadWrite)
		UStaticMeshComponent* m_pItemMesh;	//�A�C�e�����b�V��

protected:
	//�x�[�X�R���|�[�l���g
	UPROPERTY()
		USceneComponent* m_pBase;

	//�ڐG���o�ݒ莞��
	UPROPERTY()
		int m_collidedStateTime;

	//�h���N���X�Ŏg����悤�ɐݒ肷��ϐ�
protected:
	int m_collidedTimeCnt;		//�ڐG���o�J�E���g
	ITEM_STATE m_state;			//�X�e�[�g�Ǘ�
};
