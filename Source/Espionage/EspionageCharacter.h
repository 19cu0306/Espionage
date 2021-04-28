//-------------------------------------------------------------------
//クラス名：EspionageCharacter.h
//概要	　：キャラクターのクラス
//作成者　：19CU0228 野田八雲
//作成日　：2020/11/16
//更新　　：2020/11/16　クラス作成
//		 ：2020/11/24  ドア接触判定作成
//		 ：2020/11/25  鍵取得処理作成
//		 ：2020/11/27  プレイヤーが隠れる処理の作成
//		 ：2020/12/08  アイテムカウント処理作成
//		 ：2020/12/27  当たり判定修正
//		 ：2021/01/19  鍵取得処理作成二本目
//		 ：2021/01/25  モニター判定作成
//　　　　：2021/01/26　コメント追加
//　		 ：2021/01/27 perception stimuli source (sight) 追加
//		 ：2021/02/13  レーザーアクション処理作成
//		 ：2021/03/01  プレイヤー走り処理作成
//-------------------------------------------------------------------

//インクルードカード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "EspionageCharacter.generated.h"

UCLASS(config=Game)
class AEspionageCharacter : public ACharacter,public IGenericTeamAgentInterface
{

	//スコア情報格納用構造体
	struct SCORE_INFO
	{
		int score;			//スコア
		bool isHaveKey01;	//鍵の所持フラグ一本目
		bool isHaveKey02;	//鍵の所持フラグ二本目
		bool MonKeyCheck;	//モニターを使って鍵を開けたかの確認フラグ

		//コンストラクタ
		SCORE_INFO()
			:score(0)
			,isHaveKey01(false)
			,isHaveKey02(false)
			,MonKeyCheck(false)
		{}
	};

	GENERATED_BODY()

	// キャラクターの後ろに設置する用のカメラ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// 追従カメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	//コンストラクタ
	AEspionageCharacter();

	//基本回転速度（左右）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	// 基本回転速度（上下） 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//ブループリントで呼ばれる変数
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		FGenericTeamId GetTeamId()const { return TeamID; }
	//Register Character On Perception System as a stimuli source
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception Stimuli")
		class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception Stimuli")
		class UPawnNoiseEmitterComponent* NoiseEmitComp;

protected:

	//縦方向の入力メソッド
	void MoveForward(float Value);

	//横方向の入力メソッド
	void MoveRight(float Value);

	//走り用
	void UpdateRun();

	//走り終了
	void RunFinish();

	//キャラのインタラクト処理
	void Chara_Interact();

	//入力により呼び出され、指定した速度で動く
	void TurnAtRate(float Rate);

	//入力により呼び出され、カメラを指定した速度で上下する
	void LookUpAtRate(float Rate);

public:
	//カメラ更新処理
	void UpdateCamera(float DeltaTime);

	//スコア取得処理
	void GetScore(int _score);

	//鍵の取得処理
	void GetKey01(bool _bHaveKey);

	//鍵の取得処理
	void GetKey02(bool _bHaveKey);

	//プレイヤーアクションスタート
	void ActionStart();

	//プレイヤーアクション処理
	void UpdateAction(float DeltaTime);

	//モニターによって鍵を開けたかの確認用関数
	void MonitorKeyUnlock(bool _bHaveKey);

public:
	//スコアを取得
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		int GetScore()const { return m_info.score; }

	//アクションしているかのアニメーション制御用
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool isAction();

	//走っているかのアニメーション制御用
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool isRunning();

	//鍵の取得判定一本目
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool GetKey01()const { return m_info.isHaveKey01; }

	//鍵の取得判定二本目
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool GetKey02()const { return m_info.isHaveKey02; }

	//モニターによって鍵を開けたかの確認用関数
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool MonitorKeyUnlock()const { return m_info.MonKeyCheck; }

	//隠れているかの確認用変数
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsHiding() const { return m_bHiding; }

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// 毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	//カメラブームサブオブジェクトを返す関数
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//追従カメラのサブオブジェクトを返す関数
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:	

	AActor* m_pHideObject;	//隠れているオブジェクト情報

	SCORE_INFO m_info;		//スコア格納用変数

	FVector2D m_cameraRotateInput;				//カメラ回転入力量

	//UPROPERTYにすることで、ブループリント上で変数の確認、編集などができる
	//「BlueprintReadOnly」にしているため、ブループリント上で見ることだけ可能で、編集はできない
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;		//スプリングアーム

	UPROPERTY(EditAnywhere, Category = "HideObject")
		float m_HideLength;

	//走っているかの確認用フラグ
	bool m_isRunning;

	//アクション用変数
	float m_ActionMoveTime;
	float m_prevActionMove;
	float m_MovePower;

	//隠れているか？
	bool m_bHiding;

	//アクション中か？
	bool m_Actioning;
	FVector m_posBeforeMove;

	UPROPERTY(VisibleAnywhere, Category = "Team")
		FGenericTeamId TeamID;

};

