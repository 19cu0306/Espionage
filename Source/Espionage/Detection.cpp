//Project			:Espisonage
//ファイル名			:Detection.cpp
// 作成者			:19CU0306	ANH TAI
//概要				:視覚機能
//歴史				:2020/11/29 作成 PROTOTYPE
//-----------------------------------------------------------------------

#include "Detection.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SecurityPersonel.h"
#include "EspionageCharacter.h"

// Sets default values for this component's properties
UDetection::UDetection()
	:bActorInView(false)
	, bIsHaveViewOfActor(false)
	, pActorToTrace(NULL)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DetecMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dectection Mesh"));
	DetecMesh->SetCollisionProfileName("ignoreAllbutPawn");
	DetecMesh->SetGenerateOverlapEvents(true);
	DetecMesh->SetSimulatePhysics(false);
	if (GetOwner() != nullptr)
	{
		DetecMesh->SetRelativeLocationAndRotation(GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
}


// Called when the game starts
void UDetection::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
	// ...
	if (DetecMesh == nullptr)
	{

		//UE_LOG(LogTemp, Warning, TEXT("%s HAVE NO MESH COMP"), *this->GetOwner()->GetName());
	}
	else
	{
		DetecMesh->OnComponentBeginOverlap.AddDynamic(this, &UDetection::OnOverlap);
		DetecMesh->OnComponentEndOverlap.AddDynamic(this, &UDetection::OnOverlapEnd);
		DetecMesh->IgnoreActorWhenMoving(this->GetOwner(), true);
	
	}
	
}


// Called every frame
void UDetection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//アクターが可視範囲の内にいる
	if (bActorInView)
	{
		if (LineOfSight(pActorToTrace))
		{
			//do stuff
			Cast<ASecurityPersonel>(GetOwner())->SetAlertState(true);
			//UE_LOG(LogTemp, Warning, TEXT("ACTOR IS IN LINE OF SIGHT "));
		}
	}
}

void UDetection::OnOverlap(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Dectection.h COLLISION  "), *OtherActor->GetName());

	//Playerだけ
	if (OtherActor && OtherComp && OtherActor != this->GetOwner() && OtherActor->ActorHasTag("Player"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("DETECTED ACTOR %s "), *OtherActor->GetName());

		//可視か？トレーシング
		pActorToTrace = OtherActor;
		bActorInView = true;					
		SetComponentTickEnabled(true);			//処理を減る
	}
}

void UDetection::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp && OtherActor != this->GetOwner() && OtherActor->ActorHasTag("Player"))
	{
		bActorInView = false;
		bIsHaveViewOfActor = false;
		pActorToTrace = nullptr;
		SetComponentTickEnabled(false);
	}
}

bool UDetection::LineOfSight(AActor* pbActorInView)
{
	//RayCastが必要引数
	FHitResult hitResult;
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(GetOwner());
	
	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), DetecMesh->GetComponentLocation(), pbActorInView->GetActorLocation(), FVector(10, 10, 20), FRotator::ZeroRotator, ETraceTypeQuery::TraceTypeQuery_MAX,true
		,IgnoreActor, EDrawDebugTrace::ForOneFrame, hitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if (hitResult.GetActor() == pbActorInView)
	{
		bIsHaveViewOfActor = true;
		ActorLastKnowPos = hitResult.GetActor()->GetActorLocation();
		return true;
	}
	else 
	{
		bIsHaveViewOfActor = false;
		//UE_LOG(LogTemp, Warning, TEXT("PLAYER IS BlOCK BY PAWN %s "), *hitResult.GetActor()->GetName());

	}
	return false;
}

FVector UDetection::GetTraceActorPosInfo() const
{
	return ActorLastKnowPos;
}
bool UDetection::IsTraceActorInView() const
{
	return bIsHaveViewOfActor;
}