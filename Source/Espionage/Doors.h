//Project			:Espionage
//�t�@�C����			:Doors.h
// �쐬��			:19CU0306	ANH TAI
//�T�v				:�C���x���g��
//���j				:2020/10/20 �쐬	(�������m�F)
//-----------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Doors.generated.h"

class UBoxComponent;

//�h�A�[�v���p�e�B
UENUM(BlueprintType)
	enum DOOR_TYPE
	{
		SWING		UMETA(DisplayName = "Swing Door"),
		SLIDE		UMETA(DisplayName = "Slide Door"),
	};
UENUM(BlueprintType)
	enum DOOR_LOCKTYPE
	{
		DOOR_LOCK_KEY			UMETA(DisplayName = "Key Lock"),
		DOOR_LOCK_PASSWORD		UMETA(DisplayName = "Password"),
		DOOR_LOCK_NOLOCK		UMETA(DisplayName = "No Security Lock"),
	};


UCLASS()
class  ESPIONAGE_API ADoors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Doors Attachment")
		USceneComponent* pRootComponent;
	UPROPERTY(EditAnywhere, Category = "Doors Attachment")
		UStaticMeshComponent* pStaticMesh;
	UPROPERTY(EditAnywhere, Category = "Doors Attachment")
		UBoxComponent* pBoxCollider;


	//���C���
	UPROPERTY(EditAnywhere, Category = "Doors")
		bool bOpen;									//�h�A�[�J��

	//�h�A�[����
	UPROPERTY(EditAnywhere, Category = "Doors")
		float turnTime;								//�J����
	UPROPERTY(EditAnywhere, Category = "Doors")
		bool bAuto;									//�I�[�g
	UPROPERTY(EditAnywhere, Category = "Doors")
		TEnumAsByte<DOOR_TYPE>		DoorType;
	UPROPERTY(EditAnywhere, Category = "Doors")
		TEnumAsByte<DOOR_LOCKTYPE>	LockType;

	UPROPERTY(EditAnywhere, Category = "Slide Door", meta = ( EditCondition = "DoorType==DOOR_TYPE::SLIDE"))
		FVector TargetPos;																												//�^�[�Q�b�g���W�[�J���W
	UPROPERTY( EditAnywhere, Category = "Swing Door",  meta = (EditCondition = "DoorType == DOOR_TYPE::SWING"))
		float TargetAngle;																												//��]�p�[�J�p�x
	UPROPERTY(EditAnywhere, Category = "Lock", meta = (EditCondition = "LockType==DOOR_LOCKTYPE::DOOR_LOCK_KEY"))
		AActor* keyObject;																												//key
	UPROPERTY(EditAnywhere, Category = "Lock", meta = (EditCondition = "LockType==DOOR_LOCKTYPE::DOOR_LOCK_PASSWORD"))
		FString password;																												//password


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void UnlockDoor(FString _password = "", AActor* _keyObject = nullptr);				//���b�N���ꂽ�h�A�[���J����

	void TurnDoor();																	//�h�A�[��]
	void SlideDoor();																	//�h�A�[�X���C�h
	void SetState();																	//�h�A�[�J�ݒ�

		//UPROPERTY(EditAnywhere, Category = "Doors")	
	//	bool debugOpen;							//debug 
	//void debug(){ if (debugOpen) { SetState(); debugOpen = false; } }

private:
	FRotator CloseRot;		//�܂��Ԃ̊p�x
	FVector  ClosePos;		//�܂��Ԃ̍��W
	float radius;			//���a�@pivot�𐳂����Z�b�g�����炢��Ȃ�
	float angle ;			//���J�p�x
	float deltaTime;		//���J����
	bool lockState;			//���b�N���
	AActor* pCharacter;		//Character�|�C���^
	//void disableTick();

};
