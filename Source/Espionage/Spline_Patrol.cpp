//-------------------------------------------------------------------
//�N���X���FSpline_Patrol.cpp
//�T�v	�@�FAI�L�����i�x�����j�̏���
//�쐬�ҁ@�F19CU0306 ���[�@�O�G���@�A�C���@�^�C
//�쐬���@�F2020/11/16
//�X�V�@�@�F2020/11/16�@�N���X�쐬
//		�@�F2020/01/05  �v���O�����C��
//-------------------------------------------------------------------


#include "Spline_Patrol.h"
#include "Components/SplineComponent.h"

// Sets default values
ASpline_Patrol::ASpline_Patrol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pSplineComp = CreateDefaultSubobject<USplineComponent>("SplineComp");
	RootComponent = pSplineComp;
	//pSplineComp->SetDrawDebug(true);
	//pSplineComp->ScaleVisualizationWidth = 30.0f;

}

// Called when the game starts or when spawned
void ASpline_Patrol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpline_Patrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

