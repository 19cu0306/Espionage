//-------------------------------------------------------------------
//�N���X���FEspionageCharacter.h
//�T�v	�@�F�L�����N�^�[�̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�N���X�쐬
//		 �F2020/11/24  �h�A�ڐG����쐬
//		 �F2020/11/25  ���擾�����쐬
//		 �F2020/11/27  �v���C���[���B��鏈���̍쐬
//		 �F2020/12/08  �A�C�e���J�E���g�����쐬
//		 �F2020/12/27  �����蔻��C��
//		 �F2021/01/19  ���擾�����쐬��{��
//		 �F2021/01/25  ���j�^�[����쐬
//�@�@�@�@�F2021/01/26�@�R�����g�ǉ�
//�@		 �F2021/01/27 perception stimuli source (sight) �ǉ�
//		 �F2021/02/13  ���[�U�[�A�N�V���������쐬
//		 �F2021/03/01  �v���C���[���菈���쐬
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "EspionageCharacter.generated.h"

UCLASS(config=Game)
class AEspionageCharacter : public ACharacter,public IGenericTeamAgentInterface
{

	//�X�R�A���i�[�p�\����
	struct SCORE_INFO
	{
		int score;			//�X�R�A
		bool isHaveKey01;	//���̏����t���O��{��
		bool isHaveKey02;	//���̏����t���O��{��
		bool MonKeyCheck;	//���j�^�[���g���Č����J�������̊m�F�t���O

		//�R���X�g���N�^
		SCORE_INFO()
			:score(0)
			,isHaveKey01(false)
			,isHaveKey02(false)
			,MonKeyCheck(false)
		{}
	};

	GENERATED_BODY()

	// �L�����N�^�[�̌��ɐݒu����p�̃J���� 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// �Ǐ]�J����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	//�R���X�g���N�^
	AEspionageCharacter();

	//��{��]���x�i���E�j
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	// ��{��]���x�i�㉺�j 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//�u���[�v�����g�ŌĂ΂��ϐ�
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		FGenericTeamId GetTeamId()const { return TeamID; }
	//Register Character On Perception System as a stimuli source
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception Stimuli")
		class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception Stimuli")
		class UPawnNoiseEmitterComponent* NoiseEmitComp;

protected:

	//�c�����̓��̓��\�b�h
	void MoveForward(float Value);

	//�������̓��̓��\�b�h
	void MoveRight(float Value);

	//����p
	void UpdateRun();

	//����I��
	void RunFinish();

	//�L�����̃C���^���N�g����
	void Chara_Interact();

	//���͂ɂ��Ăяo����A�w�肵�����x�œ���
	void TurnAtRate(float Rate);

	//���͂ɂ��Ăяo����A�J�������w�肵�����x�ŏ㉺����
	void LookUpAtRate(float Rate);

public:
	//�J�����X�V����
	void UpdateCamera(float DeltaTime);

	//�X�R�A�擾����
	void GetScore(int _score);

	//���̎擾����
	void GetKey01(bool _bHaveKey);

	//���̎擾����
	void GetKey02(bool _bHaveKey);

	//�v���C���[�A�N�V�����X�^�[�g
	void ActionStart();

	//�v���C���[�A�N�V��������
	void UpdateAction(float DeltaTime);

	//���j�^�[�ɂ���Č����J�������̊m�F�p�֐�
	void MonitorKeyUnlock(bool _bHaveKey);

public:
	//�X�R�A���擾
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		int GetScore()const { return m_info.score; }

	//�A�N�V�������Ă��邩�̃A�j���[�V��������p
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool isAction();

	//�����Ă��邩�̃A�j���[�V��������p
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool isRunning();

	//���̎擾�����{��
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool GetKey01()const { return m_info.isHaveKey01; }

	//���̎擾�����{��
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool GetKey02()const { return m_info.isHaveKey02; }

	//���j�^�[�ɂ���Č����J�������̊m�F�p�֐�
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool MonitorKeyUnlock()const { return m_info.MonKeyCheck; }

	//�B��Ă��邩�̊m�F�p�ϐ�
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsHiding() const { return m_bHiding; }

protected:
	// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	// ���t���[���̍X�V����
	virtual void Tick(float DeltaTime) override;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	//�J�����u�[���T�u�I�u�W�F�N�g��Ԃ��֐�
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//�Ǐ]�J�����̃T�u�I�u�W�F�N�g��Ԃ��֐�
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:	

	AActor* m_pHideObject;	//�B��Ă���I�u�W�F�N�g���

	SCORE_INFO m_info;		//�X�R�A�i�[�p�ϐ�

	FVector2D m_cameraRotateInput;				//�J������]���͗�

	//UPROPERTY�ɂ��邱�ƂŁA�u���[�v�����g��ŕϐ��̊m�F�A�ҏW�Ȃǂ��ł���
	//�uBlueprintReadOnly�v�ɂ��Ă��邽�߁A�u���[�v�����g��Ō��邱�Ƃ����\�ŁA�ҏW�͂ł��Ȃ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;		//�X�v�����O�A�[��

	UPROPERTY(EditAnywhere, Category = "HideObject")
		float m_HideLength;

	//�����Ă��邩�̊m�F�p�t���O
	bool m_isRunning;

	//�A�N�V�����p�ϐ�
	float m_ActionMoveTime;
	float m_prevActionMove;
	float m_MovePower;

	//�B��Ă��邩�H
	bool m_bHiding;

	//�A�N�V���������H
	bool m_Actioning;
	FVector m_posBeforeMove;

	UPROPERTY(VisibleAnywhere, Category = "Team")
		FGenericTeamId TeamID;

};

