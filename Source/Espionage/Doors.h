//Project			:Espionage
//ファイル名			:Doors.h
// 作成者			:19CU0306	ANH TAI
//概要				:インベントリ
//歴史				:2020/10/20 作成	(動き未確認)
//-----------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Doors.generated.h"

class UBoxComponent;

//ドアープロパティ
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


	//正気状態
	UPROPERTY(EditAnywhere, Category = "Doors")
		bool bOpen;									//ドアー開閉

	//ドアー特性
	UPROPERTY(EditAnywhere, Category = "Doors")
		float turnTime;								//開閉時間
	UPROPERTY(EditAnywhere, Category = "Doors")
		bool bAuto;									//オート
	UPROPERTY(EditAnywhere, Category = "Doors")
		TEnumAsByte<DOOR_TYPE>		DoorType;
	UPROPERTY(EditAnywhere, Category = "Doors")
		TEnumAsByte<DOOR_LOCKTYPE>	LockType;

	UPROPERTY(EditAnywhere, Category = "Slide Door", meta = ( EditCondition = "DoorType==DOOR_TYPE::SLIDE"))
		FVector TargetPos;																												//ターゲット座標ー開座標
	UPROPERTY( EditAnywhere, Category = "Swing Door",  meta = (EditCondition = "DoorType == DOOR_TYPE::SWING"))
		float TargetAngle;																												//回転角ー開角度
	UPROPERTY(EditAnywhere, Category = "Lock", meta = (EditCondition = "LockType==DOOR_LOCKTYPE::DOOR_LOCK_KEY"))
		AActor* keyObject;																												//key
	UPROPERTY(EditAnywhere, Category = "Lock", meta = (EditCondition = "LockType==DOOR_LOCKTYPE::DOOR_LOCK_PASSWORD"))
		FString password;																												//password


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void UnlockDoor(FString _password = "", AActor* _keyObject = nullptr);				//ロックされたドアーを開ける

	void TurnDoor();																	//ドアー回転
	void SlideDoor();																	//ドアースライド
	void SetState();																	//ドアー開閉設定

		//UPROPERTY(EditAnywhere, Category = "Doors")	
	//	bool debugOpen;							//debug 
	//void debug(){ if (debugOpen) { SetState(); debugOpen = false; } }

private:
	FRotator CloseRot;		//閉まる状態の角度
	FVector  ClosePos;		//閉まる状態の座標
	float radius;			//半径　pivotを正しくセットしたらいらない
	float angle ;			//仮開閉角度
	float deltaTime;		//仮開閉時間
	bool lockState;			//ロック状態
	AActor* pCharacter;		//Characterポインタ
	//void disableTick();

};
