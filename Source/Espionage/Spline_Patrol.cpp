//-------------------------------------------------------------------
//クラス名：Spline_Patrol.cpp
//概要	　：AIキャラ（警備員）の処理
//作成者　：19CU0306 ヴー　グエン　アイン　タイ
//作成日　：2020/11/16
//更新　　：2020/11/16　クラス作成
//		　：2020/01/05  プログラム修正
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

