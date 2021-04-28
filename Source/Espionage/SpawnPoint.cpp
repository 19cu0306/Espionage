//-------------------------------------------------------------------
//�N���X���FSpawnPoint.cpp
//�T�v	�@�F�v���C���[�����ʒu�i�X�^�[�g�n�_�j�̏���
//�쐬�ҁ@�F19CU0306 ���[�@�O�G���@�A�C���@�^�C
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�N���X�쐬
//		�@�F2020/01/05  �v���O�����C��
//-------------------------------------------------------------------

//�C���N���[�h
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

//�R���X�g���N�^
ASpawnPoint::ASpawnPoint():DefaultSpawnPoint(false)
{
 	//���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ��ł���B
	PrimaryActorTick.bCanEverTick = false;

	//�R���W��������p�{�b�N�X�R���|�[�l���g����
	pBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	pBoxCollider->SetupAttachment(RootComponent);
}

//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (Character == NULL) { 
		//Character = LoadClass<Aplayer01>(NULL, TEXT("player01"), NULL, LOAD_None, NULL);
		//UE_LOG(LogClass, Warning, TEXT("Actor is NOT SET"));
	}

	SpawnPointNum = SpawnPoint.Num();
	
	if (SpawnPointNum <= 0)
	{
		MasterSpawnController = false;
	}
	else MasterSpawnController = true;



}

//���t���[���X�V����
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnPoint::setSpawn(unsigned int num)
{
	if(num<SpawnPointNum)
		Character->SetActorLocationAndRotation(SpawnPoint[num]->GetActorLocation(), SpawnPoint[num]->GetActorRotation());
	else 	Character->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());
}

FVector ASpawnPoint::getSpawnPos(unsigned int num)
{
	if (num < SpawnPointNum)
		return SpawnPoint[num]->GetActorLocation();
	else return this->GetActorLocation();
}
