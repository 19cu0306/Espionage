//-------------------------------------------------------------------
//�N���X���FSpawnPoint.h
//�T�v	�@�F�v���C���[�����ʒu�i�X�^�[�g�n�_�j�̏���
//�쐬�ҁ@�F19CU0306 ���[�@�O�G���@�A�C���@�^�C
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�N���X�쐬
//		�@�F2020/01/05  �v���O�����C��
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "SpawnPoint.generated.h"

//�O���錾
class UBoxComponent;

UCLASS()
class ESPIONAGE_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// �R���X�g���N�^
	ASpawnPoint();

protected:
	//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;
public:	
	//���t���[���X�V����
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "SpawnPoint")
		bool DefaultSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "SubSpawnPoints")
		TArray<ASpawnPoint*> SpawnPoint;

	UPROPERTY(EditAnywhere, Category = "CharacterProb")
		ACharacter* Character;

private:
	//�錾
	bool MasterSpawnController;
	FVector getSpawnPos(unsigned int num =0);
	void setSpawn(unsigned int num =0);
	unsigned int SpawnPointNum;
	UBoxComponent* pBoxCollider;			//editor indicator

};
