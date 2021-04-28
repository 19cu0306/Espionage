//-------------------------------------------------------------------
//�N���X���FEspionageCharacter.cpp
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
//�@ 	 �F2021/01/27 perception stimuli source (sight) �ǉ�
//		 �F2021/02/13  ���[�U�[�A�N�V���������쐬
//		 �F2021/03/01  �v���C���[���菈���쐬
//-------------------------------------------------------------------

//�C���N���[�h
#include "EspionageCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Doors.h"
#include "Monitor.h"
#include"HideObjectBase.h"
#include"DrawDebugHelpers.h"

//ai�n�@�C���N���[�h
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Touch.h"
#include "Perception/AISense_Sight.h"
#include "GenericTeamAgentInterface.h"
#include "Components/PawnNoiseEmitterComponent.h"

#define MOVE (m_ActionMoveTime * m_ActionMoveTime *(m_ActionMoveTime * m_MovePower))

//////////////////////////////////////////////////////////////////////////
// AEspionageCharacter
//�R���X�g���N�^
AEspionageCharacter::AEspionageCharacter()
	:m_info()
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_pSpringArm(NULL)
	, m_HideLength(80.f)
	, m_isRunning(false)
	, m_ActionMoveTime(0.f)
	, m_prevActionMove(0.f)
	, m_MovePower(450.f)
	, m_bHiding(false)
	, m_Actioning(false)
{
	// �R���W�����T�C�Y
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//���͂ɂ��^�[�����[�g�ݒ�
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//�R���g���[���[��]���ɉ�]���Ȃ��悤�ɂ���
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//�L��������ݒ�
	GetCharacterMovement()->bOrientRotationToMovement = true; // ���͂ɂ���]
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ��]���x

	//�J�����u�[����ݒ肷��i��Q��������ꍇ�A�J�������v���C���[�Ɍ����킹��j
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // �L�����N�^�[�ƃJ�����̋���	
	CameraBoom->bUsePawnControlRotation = true; // �R���g���[���[�ɂ��A�[������]

	//�J�����쐬
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // �J�������u�[���̒[�Ɏ��t���A�R���g���[���[�̌����ɍ��킹����
	FollowCamera->bUsePawnControlRotation = false; //�J�������A�[���ɑ΂��ĉ�]���Ȃ��悤�ɂ���

	//register to perception system as stimuli source
	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));
	////PerceptionStimuliSource->RegisterForSense(UAISense_Hearing::StaticClass());
	PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionStimuliSource->RegisterForSense(UAISense_Touch::StaticClass());

	SetGenericTeamId(0);
	NoiseEmitComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("NoiseEmitter");
}

//////////////////////////////////////////////////////////////////////////
// Input
//�e���͊֌W�Ƃ̃o�C���h����
void AEspionageCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	//�v���C���[�̓��́i�ړ��j
	PlayerInputComponent->BindAxis("MoveForward", this, &AEspionageCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEspionageCharacter::MoveRight);

	//�v���C���[�̓��́i��]�j
	PlayerInputComponent->BindAxis("TurnRate", this, &AEspionageCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEspionageCharacter::LookUpAtRate);
	//�C���^���N�V����
	InputComponent->BindAction("Interact", IE_Pressed, this, &AEspionageCharacter::Chara_Interact);

	InputComponent->BindAction("Run", IE_Pressed, this, &AEspionageCharacter::UpdateRun);
	InputComponent->BindAction("Run", IE_Released, this, &AEspionageCharacter::RunFinish);

}
//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AEspionageCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Tick�֐�����U������
	PrimaryActorTick.SetTickFunctionEnable(false);
}

//���t���[���X�V����
void AEspionageCharacter::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	//�v���C���[�A�N�V��������
	UpdateAction(_deltaTime);
}

//�J�����X�V����
void AEspionageCharacter::UpdateCamera(float _deltaTime)
{
	//���[�g�I�u�W�F�N�g�𒆐S�ɁA�X�v�����O�A�[���ɂ��Ă���J��������]������
	USpringArmComponent* pSpringArm = m_pSpringArm;
	if (pSpringArm != NULL)
	{
		//���݂�FPS�𑪒�
		float fps = 1.0f / _deltaTime;

		//�����������Ă��A��葬�x�ŃJ���������l�ɕ␳
		float rotateCorrection = 60.f / fps;

		//�J�����̐V�����p�x�����߂�
		//���݂̊p�x���擾
		FRotator NewRotation = m_pSpringArm->GetRelativeRotation();

		//Yaw�͓��͂�������
		NewRotation.Yaw += m_cameraRotateInput.X * rotateCorrection;

		//�V�����p�x�𔽉f
		pSpringArm->SetRelativeRotation(NewRotation);
	}
}

//�v���C���[�̓��́i��]�j
void AEspionageCharacter::TurnAtRate(float Rate)
{
	// ���[�g��񂩂�t���[�����Ԃ��v�Z
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
//�v���C���[�̓��́i��]�j
void AEspionageCharacter::LookUpAtRate(float Rate)
{
	// ���[�g��񂩂�t���[�����Ԃ��v�Z
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
//�v���C���[�̓��́i�ړ��j
void AEspionageCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//���͂��ꂽ�����𒲂ׂ�
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//�x�N�g���擾
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
//�v���C���[�̓��́i�ړ��j
void AEspionageCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//���͂��ꂽ�����𒲂ׂ�
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//�x�N�g���擾
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//���̕����ɓ�����ǉ�
		AddMovementInput(Direction, Value);
	}
}
//����p�i�{�^����������Ă���Ƃ��j
void AEspionageCharacter::UpdateRun()
{
	if ((Controller != NULL))
	{
		//�v���C���[�̃X�s�[�h��2�{�ɂ���
		GetCharacterMovement()->MaxWalkSpeed *= 2.0;
		m_isRunning = true;
	}
}

//����I���i�{�^����b�������j
void AEspionageCharacter::RunFinish()
{
	if (m_isRunning == true)
	{
		//�v���C���[�̃X�s�[�h��1/2�ɂ���i���̃X�s�[�h�ɖ߂��j
		GetCharacterMovement()->MaxWalkSpeed /= 2.0;
		m_isRunning = false;
	}
}

//�X�R�A�l������
void AEspionageCharacter::GetScore(int _score)
{
	//�f�o�b�N�m�F�p
	//UE_LOG(LogTemp, Warning, TEXT("before get score %d"), m_info.score);

	//�A�C�e���擾�ɂ��ω����X�R�A���ɔ��f
	m_info.score += _score;

	//�f�o�b�N�m�F�p
	//UE_LOG(LogTemp, Warning, TEXT("after get item score > %d"), m_info.score);
}

//�v���C���[�A�N�V�����X�^�[�g
void AEspionageCharacter::ActionStart()
{
	if (m_Actioning == false)
	{
		m_Actioning = true;
		m_posBeforeMove = GetActorLocation();
	}
}

//�v���C���[�A�N�V��������
void AEspionageCharacter::UpdateAction(float Deltatime)
{
	//�A�N�V�����t���O���m�F���Ă���
	if (m_Actioning)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HitActionPoint"));

		//�A�N�V�������Ԃ��v��
		m_ActionMoveTime += Deltatime;

		//�V�������W���i�[
		FVector newPos = GetActorLocation();

		//UE_LOG(LogTemp, Warning, TEXT("Count"), m_ActionMoveTime);
	
		//�w�肵���A�N�V�������Ԃ𒴂�����
		if (m_ActionMoveTime > 0.0f)
		{
			//���W���ς��u�Ԃ̂݁A�v���C���[�̓����蔻�������
			this->SetActorEnableCollision(false);
			SetActorLocation(FVector((m_ActionMoveTime * 250) + m_posBeforeMove.X, newPos.Y, newPos.Z), true);
			this->SetActorEnableCollision(true);

			//�v�����Ԃ����Z�b�g
			Deltatime = 0.0f;

			if (m_ActionMoveTime > 1.6f)
			{
				//�A�N�V�������ԃ��Z�b�g
				m_ActionMoveTime = 0.0f;
				//�A�N�V�����A�j���[�V�����I��
				m_Actioning = false;
			}
		}
	}
}

//�A�N�V�������Ă��邩�̃A�j���[�V��������p
bool AEspionageCharacter::isAction()
{
	//�A�N�V�����t���O��false�Ȃ�A�������Ȃ�
	if (m_Actioning != true) { return false; }

	//move�ʂ̐ݒ�
	float move = MOVE;

	//�A�j���[�V��������p
	bool ret = false;

	//move�̕�����Ȃ�A�A�j���[�V�����Đ�
	(move > m_prevActionMove) ? (ret = true) : (ret = false);
	//���ʂ�Ԃ�
	return ret;
}

//�����Ă��邩�̃A�j���[�V��������p
bool AEspionageCharacter::isRunning()
{
	bool ret = false;

	//�����Ă��邩�̃t���O�ɂ���āA�A�j���[�V�����𐧌�
	(m_isRunning) ? (ret = true) : (ret = false);
	return ret;
}

//���̎擾����(��{��)
void AEspionageCharacter::GetKey01(bool _bHaveKey)
{
	m_info.isHaveKey01 = _bHaveKey;

	//UE_LOG(LogTemp, Warning, TEXT("HIT KEY "));
}

//���̎擾����i��{�ځj
void AEspionageCharacter::GetKey02(bool _bHaveKey)
{
	m_info.isHaveKey02 = _bHaveKey;
	//UE_LOG(LogTemp, Warning, TEXT("HIT KEY 02 "));
}

//���j�^�[�ɂ���Č����J�������̊m�F�p�֐�
void AEspionageCharacter::MonitorKeyUnlock(bool _bHaveKey)
{
	m_info.MonKeyCheck = _bHaveKey;
	//UE_LOG(LogTemp, Warning, TEXT("MONITOR KEY 02 "));
}

//�L�����̃C���^���N�g
void AEspionageCharacter::Chara_Interact()
{
	//���b�V���擾
	USkeletalMeshComponent* pMeshComp = GetMesh();
	if (pMeshComp != NULL)
	{
		//���C���΂�
		//�n�_
		FVector Start = GetActorLocation();
		//�I�_
		FVector End = GetActorLocation() + pMeshComp->GetRightVector() * 250.f;

		//���C��`�悷��
		//DrawDebugLine(GetWorld(), Start, End, FColor().Red, false, 5.0f, 0, 1);

		//�R���W��������Ŗ������钸�ڂ��w��i����͂���Actor�������g�Athis�|�C���^�Ŏw��)
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//�q�b�g�����I�u�W�F�N�g���i�[�ϐ�
		FHitResult OutHit;

		//�R���W��������p
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);

		//�q�b�g����I�u�W�F�N�g������
		if (isHit)
		{
			//�q�b�g�����A�N�^�[�̃^�O�ɂ���ď�����ύX����
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor"));
			//�B���p�I�u�W�F�N�g
			if (OutHit.GetActor()->ActorHasTag("CanHide"))
			{
				AHideObjectBase* tmp = Cast<AHideObjectBase>(OutHit.GetActor());
				m_bHiding = m_bHiding ? false : true;
				tmp->SetHide(m_bHiding);
				//UE_LOG(LogTemp, Warning, TEXT("Hit Actor TagCanHide, isHide %d"),m_bHiding);
			}
			//�v���C���[�A�N�V�����p
			if (OutHit.GetActor()->ActorHasTag("ActionPoint"))
			{
				ActionStart();
				PrimaryActorTick.SetTickFunctionEnable(true);
				//UE_LOG(LogTemp, Warning, TEXT("Action Start %d"));
			}
		}
		//�����Ȃ���΁A���O��\��
		//else UE_LOG(LogTemp, Warning, TEXT("no Hit"));
	}
}
