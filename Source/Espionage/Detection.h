//Project			:Espisonage
//ファイル名			:Detection.h
// 作成者			:19CU0306	ANH TAI
//概要				:視覚機能
//歴史				:2020/11/29 作成 PROTOTYPE
//-----------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Detection.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESPIONAGE_API UDetection : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDetection();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//オバラップ関数
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//StaticMesh
	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* DetecMesh;

	FVector GetTraceActorPosInfo() const;
	bool IsTraceActorInView() const;

private:
	bool LineOfSight(AActor* pTracingActor);		//アクターが見えるか？
	bool bActorInView;								//アクタートレーシングをするか？
	bool bIsHaveViewOfActor;						//アクターが見える
	FVector ActorLastKnowPos;

	AActor* pActorToTrace;							//トレーシングするアクター

};
