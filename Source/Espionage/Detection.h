//Project			:Espisonage
//�t�@�C����			:Detection.h
// �쐬��			:19CU0306	ANH TAI
//�T�v				:���o�@�\
//���j				:2020/11/29 �쐬 PROTOTYPE
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

	//�I�o���b�v�֐�
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
	bool LineOfSight(AActor* pTracingActor);		//�A�N�^�[�������邩�H
	bool bActorInView;								//�A�N�^�[�g���[�V���O�����邩�H
	bool bIsHaveViewOfActor;						//�A�N�^�[��������
	FVector ActorLastKnowPos;

	AActor* pActorToTrace;							//�g���[�V���O����A�N�^�[

};
