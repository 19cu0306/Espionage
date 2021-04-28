//Project			:Espionage
//�t�@�C����		:Doors.cpp
// �쐬��			:19CU0306	ANH TAI
//�T�v				:�C���x���g��
//���j				:2020/10/20 �쐬	
//-----------------------------------------------------------------------
//�쐬��			:19CU0228	��c���_
//�X�V����			:2021/01/19 �h�A�����ǉ�(�����ƂɃh�A���J����)
//  				:2021/01/26 �h�A�����ǉ� �i���Ȃ��p�h�A�j
//-----------------------------------------------------------------------


//�C���N���[�h
#include "Doors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include"EspionageCharacter.h"
#include"GetKey.h"

//�R���X�g���N�^
ADoors::ADoors()
	:pStaticMesh(NULL)
	,pBoxCollider(NULL)
	, bOpen(false)
	, turnTime(1)
	, TargetPos(NULL)
	, TargetAngle(120.f)
	, CloseRot(NULL)
	, radius(0.f)
	, angle(0)
	, deltaTime(0.f)
	, lockState(false)
	,pCharacter(NULL)
{
	//�A�N�^�[��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ��ł���B
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Doors");

	pRootComponent= CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = pRootComponent;
	pBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	pBoxCollider->SetupAttachment(RootComponent);
	pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	pStaticMesh->SetupAttachment(RootComponent);
	
	pBoxCollider->SetCollisionProfileName("OverlapOnlyPawn");
}

//�Q�[���J�n���A�܂��͐������ɌĂ΂�鏈��
void ADoors::BeginPlay()
{
	Super::BeginPlay();

	if (pBoxCollider != NULL)
	{
		pBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ADoors::OnOverlapBegin);
		pBoxCollider->OnComponentEndOverlap.AddDynamic(this, &ADoors::OnOverlapEnd);
	}

	SetActorTickEnabled(false);

	if (password != "" || keyObject != nullptr)
	{
		lockState = true;
	}
	else lockState = false;


	CloseRot = pStaticMesh->GetComponentRotation();
	ClosePos = pStaticMesh->GetComponentLocation();
}

//���t���[���X�V����
void ADoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�h�A�[�J����̎���
	deltaTime += DeltaTime;

	//���Ԑ���
	if (deltaTime >= turnTime)
	{
		deltaTime = turnTime;
	}

	//�h�A�̎��
	switch (DoorType)
	{
	case DOOR_TYPE::SWING:
		TurnDoor();
		break;
	case DOOR_TYPE::SLIDE:
		SlideDoor();
		break;
	}
	/*switch (LockType)
	{
	case DOOR_LOCK_NOLOCK:
		break;
	case DOOR_LOCK_KEY:
		if (lockState == false)
		{
			UnlockDoor();
		}
		break;
	case DOOR_LOCK_PASSWORD:
		break;
	}*/
}

void ADoors::SetState()
{
	if (deltaTime >= turnTime || deltaTime==0)
	{
		deltaTime = 0;
	}
	else { deltaTime = turnTime - deltaTime;  }

	if (lockState)
	{
		bOpen = false;
	}
	else
	{
		bOpen = bOpen ? false : true;
		SetActorTickEnabled(true);
	}

}

//��]�h�A
void ADoors::TurnDoor()
{
	angle = FMath().Lerp(CloseRot.Yaw, TargetAngle + CloseRot.Yaw, bOpen ? deltaTime / turnTime : 1 - deltaTime / turnTime);
	float rad = FMath().DegreesToRadians(angle);

	FRotator newRotation = FRotator(CloseRot.Roll, angle, CloseRot.Pitch);

	pStaticMesh->SetWorldLocationAndRotation(ClosePos, newRotation);

	FRotator tmpRot = pStaticMesh->GetComponentRotation();

	if (tmpRot.Yaw >= TargetAngle + CloseRot.Yaw && bOpen || tmpRot.Yaw == 0 && bOpen == false)
	{
		SetActorTickEnabled(false); //comment out this to test or debug
		deltaTime = 0;
	}
}

//�X���C�h�h�A
void ADoors::SlideDoor()
{
	pStaticMesh->SetWorldLocationAndRotation(FMath().Lerp(ClosePos, TargetPos, bOpen ? deltaTime / turnTime : 1 - deltaTime / turnTime), GetActorRotation());

	FVector tmpPos = pStaticMesh->GetComponentLocation();
	if (tmpPos == TargetPos && bOpen || tmpPos == ClosePos && bOpen == false)
	{
		SetActorTickEnabled(false); //comment out this to test or debug
		deltaTime = 0;
	}
}

//�h�A�̌�����
void ADoors::UnlockDoor(FString _password, AActor* _keyObject)
{
	if (_password == password|| _keyObject == keyObject)
	{
		//UE_LOG(LogTemp, Warning, TEXT("overlap start"));
	}
}

//Actor�ƃI�[�o�[���b�v�������̏���
void ADoors::OnOverlapBegin(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult)
{
	//�I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			//�ǂ̃h�A���ɂ���ď��������ς��
			//UE_LOG(LogTemp, Warning, TEXT("overlap start"));;
			pCharacter = Cast<AEspionageCharacter,AActor>(OtherActor);
			AEspionageCharacter* tmp = Cast<AEspionageCharacter>(OtherActor);
			if (bAuto && this->ActorHasTag("Doors"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
			else if (bAuto && tmp->GetKey01() == true && this->ActorHasTag("Door01"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
			else if (bAuto && tmp->GetKey02() == true && this->ActorHasTag("Door02"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
			else if (bAuto && tmp->MonitorKeyUnlock() == true && this->ActorHasTag("Door03"))
			{
				//pCharacter.
				if (deltaTime > 0 && deltaTime < turnTime && bOpen == false)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = true;
				SetActorTickEnabled(true);
			}
		}
	}
}

//�I�[�o�[���b�v���I�������Ƃ��̏���
void ADoors::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	//UE_LOG(LogTemp, Warning, TEXT("overlap start"));;
	pCharacter = Cast<AEspionageCharacter, AActor>(OtherActor);
	AEspionageCharacter* tmp = Cast<AEspionageCharacter>(OtherActor);

	//�h�A�̎�ނɂ���ď��������ς��B
	//UE_LOG(LogTemp, Warning, TEXT("overlap end"));
	if (tmp != nullptr)
	{
		if (bAuto)
		{
			//�h�A01�i��l�̕����j
			if (tmp->GetKey01() == true && this->ActorHasTag("Door01"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
			//�h�A02�i�R���g���[�����[���j
			else if (tmp->GetKey02() == true && this->ActorHasTag("Door02"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
			//�h�A03�i�R���g���[�����[���ɂ�錮�̉����p�j
			else if (tmp->MonitorKeyUnlock() == true && this->ActorHasTag("Door03"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
			//���Ȃ��h�A
			else if (this->ActorHasTag("Doors"))
			{
				if (deltaTime > 0 && deltaTime < turnTime && bOpen)
				{
					deltaTime = turnTime - deltaTime;
				}
				else deltaTime = 0;

				bOpen = false;
				SetActorTickEnabled(true);
			}
		}
	}

}