//-------------------------------------------------------------------
//クラス名：EspionageCharacter.cpp
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
//　 	 ：2021/01/27 perception stimuli source (sight) 追加
//		 ：2021/02/13  レーザーアクション処理作成
//		 ：2021/03/01  プレイヤー走り処理作成
//-------------------------------------------------------------------

//インクルード
#include "EspionageCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Doors.h"
#include "Monitor.h"
#include"HideObjectBase.h"
#include"DrawDebugHelpers.h"

//ai系　インクルード
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Touch.h"
#include "Perception/AISense_Sight.h"
#include "GenericTeamAgentInterface.h"
#include "Components/PawnNoiseEmitterComponent.h"

#define MOVE (m_ActionMoveTime * m_ActionMoveTime *(m_ActionMoveTime * m_MovePower))

//////////////////////////////////////////////////////////////////////////
// AEspionageCharacter
//コンストラクタ
AEspionageCharacter::AEspionageCharacter()
	:m_info()
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_pSpringArm(NULL)
	, m_HideLength(80.f)
	, m_isRunning(false)
	, m_ActionMoveTime(0.f)
	, m_prevActionMove(0.f)
	, m_MovePower(450.f)
	, m_bHiding(false)
	, m_Actioning(false)
{
	// コリジョンサイズ
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//入力によるターンレート設定
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//コントローラー回転中に回転しないようにする
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//キャラ動作設定
	GetCharacterMovement()->bOrientRotationToMovement = true; // 入力により回転
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 回転速度

	//カメラブームを設定する（障害物がある場合、カメラをプレイヤーに向かわせる）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // キャラクターとカメラの距離	
	CameraBoom->bUsePawnControlRotation = true; // コントローラーによりアームを回転

	//カメラ作成
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // カメラをブームの端に取り付け、コントローラーの向きに合わせ調整
	FollowCamera->bUsePawnControlRotation = false; //カメラがアームに対して回転しないようにする

	//register to perception system as stimuli source
	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));
	////PerceptionStimuliSource->RegisterForSense(UAISense_Hearing::StaticClass());
	PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionStimuliSource->RegisterForSense(UAISense_Touch::StaticClass());

	SetGenericTeamId(0);
	NoiseEmitComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("NoiseEmitter");
}

//////////////////////////////////////////////////////////////////////////
// Input
//各入力関係とのバインド処理
void AEspionageCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	//プレイヤーの入力（移動）
	PlayerInputComponent->BindAxis("MoveForward", this, &AEspionageCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEspionageCharacter::MoveRight);

	//プレイヤーの入力（回転）
	PlayerInputComponent->BindAxis("TurnRate", this, &AEspionageCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEspionageCharacter::LookUpAtRate);
	//インタラクション
	InputComponent->BindAction("Interact", IE_Pressed, this, &AEspionageCharacter::Chara_Interact);

	InputComponent->BindAction("Run", IE_Pressed, this, &AEspionageCharacter::UpdateRun);
	InputComponent->BindAction("Run", IE_Released, this, &AEspionageCharacter::RunFinish);

}
//ゲームスタート時、または生成時に呼ばれる処理
void AEspionageCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Tick関数を一旦無効化
	PrimaryActorTick.SetTickFunctionEnable(false);
}

//毎フレーム更新処理
void AEspionageCharacter::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	//プレイヤーアクション処理
	UpdateAction(_deltaTime);
}

//カメラ更新処理
void AEspionageCharacter::UpdateCamera(float _deltaTime)
{
	//ルートオブジェクトを中心に、スプリングアームについているカメラを回転させる
	USpringArmComponent* pSpringArm = m_pSpringArm;
	if (pSpringArm != NULL)
	{
		//現在のFPSを測定
		float fps = 1.0f / _deltaTime;

		//処理落ちしても、一定速度でカメラが回る様に補正
		float rotateCorrection = 60.f / fps;

		//カメラの新しい角度を求める
		//現在の角度を取得
		FRotator NewRotation = m_pSpringArm->GetRelativeRotation();

		//Yawは入力した分回す
		NewRotation.Yaw += m_cameraRotateInput.X * rotateCorrection;

		//新しい角度を反映
		pSpringArm->SetRelativeRotation(NewRotation);
	}
}

//プレイヤーの入力（回転）
void AEspionageCharacter::TurnAtRate(float Rate)
{
	// レート情報からフレーム時間を計算
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
//プレイヤーの入力（回転）
void AEspionageCharacter::LookUpAtRate(float Rate)
{
	// レート情報からフレーム時間を計算
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
//プレイヤーの入力（移動）
void AEspionageCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//入力された方向を調べる
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//ベクトル取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
//プレイヤーの入力（移動）
void AEspionageCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//入力された方向を調べる
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//ベクトル取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//その方向に動きを追加
		AddMovementInput(Direction, Value);
	}
}
//走り用（ボタンが押されているとき）
void AEspionageCharacter::UpdateRun()
{
	if ((Controller != NULL))
	{
		//プレイヤーのスピードを2倍にする
		GetCharacterMovement()->MaxWalkSpeed *= 2.0;
		m_isRunning = true;
	}
}

//走り終了（ボタンを話した時）
void AEspionageCharacter::RunFinish()
{
	if (m_isRunning == true)
	{
		//プレイヤーのスピードを1/2にする（元のスピードに戻す）
		GetCharacterMovement()->MaxWalkSpeed /= 2.0;
		m_isRunning = false;
	}
}

//スコア獲得処理
void AEspionageCharacter::GetScore(int _score)
{
	//デバック確認用
	//UE_LOG(LogTemp, Warning, TEXT("before get score %d"), m_info.score);

	//アイテム取得による変化をスコア情報に反映
	m_info.score += _score;

	//デバック確認用
	//UE_LOG(LogTemp, Warning, TEXT("after get item score > %d"), m_info.score);
}

//プレイヤーアクションスタート
void AEspionageCharacter::ActionStart()
{
	if (m_Actioning == false)
	{
		m_Actioning = true;
		m_posBeforeMove = GetActorLocation();
	}
}

//プレイヤーアクション処理
void AEspionageCharacter::UpdateAction(float Deltatime)
{
	//アクションフラグを確認してから
	if (m_Actioning)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HitActionPoint"));

		//アクション時間を計測
		m_ActionMoveTime += Deltatime;

		//新しい座標を格納
		FVector newPos = GetActorLocation();

		//UE_LOG(LogTemp, Warning, TEXT("Count"), m_ActionMoveTime);
	
		//指定したアクション時間を超えたら
		if (m_ActionMoveTime > 0.0f)
		{
			//座標が変わる瞬間のみ、プレイヤーの当たり判定を消去
			this->SetActorEnableCollision(false);
			SetActorLocation(FVector((m_ActionMoveTime * 250) + m_posBeforeMove.X, newPos.Y, newPos.Z), true);
			this->SetActorEnableCollision(true);

			//計測時間をリセット
			Deltatime = 0.0f;

			if (m_ActionMoveTime > 1.6f)
			{
				//アクション時間リセット
				m_ActionMoveTime = 0.0f;
				//アクションアニメーション終了
				m_Actioning = false;
			}
		}
	}
}

//アクションしているかのアニメーション制御用
bool AEspionageCharacter::isAction()
{
	//アクションフラグがfalseなら、処理しない
	if (m_Actioning != true) { return false; }

	//move量の設定
	float move = MOVE;

	//アニメーション判定用
	bool ret = false;

	//moveの方が上なら、アニメーション再生
	(move > m_prevActionMove) ? (ret = true) : (ret = false);
	//結果を返す
	return ret;
}

//走っているかのアニメーション制御用
bool AEspionageCharacter::isRunning()
{
	bool ret = false;

	//走っているかのフラグによって、アニメーションを制御
	(m_isRunning) ? (ret = true) : (ret = false);
	return ret;
}

//鍵の取得判定(一本目)
void AEspionageCharacter::GetKey01(bool _bHaveKey)
{
	m_info.isHaveKey01 = _bHaveKey;

	//UE_LOG(LogTemp, Warning, TEXT("HIT KEY "));
}

//鍵の取得判定（二本目）
void AEspionageCharacter::GetKey02(bool _bHaveKey)
{
	m_info.isHaveKey02 = _bHaveKey;
	//UE_LOG(LogTemp, Warning, TEXT("HIT KEY 02 "));
}

//モニターによって鍵を開けたかの確認用関数
void AEspionageCharacter::MonitorKeyUnlock(bool _bHaveKey)
{
	m_info.MonKeyCheck = _bHaveKey;
	//UE_LOG(LogTemp, Warning, TEXT("MONITOR KEY 02 "));
}

//キャラのインタラクト
void AEspionageCharacter::Chara_Interact()
{
	//メッシュ取得
	USkeletalMeshComponent* pMeshComp = GetMesh();
	if (pMeshComp != NULL)
	{
		//レイを飛ばす
		//始点
		FVector Start = GetActorLocation();
		//終点
		FVector End = GetActorLocation() + pMeshComp->GetRightVector() * 250.f;

		//レイを描画する
		//DrawDebugLine(GetWorld(), Start, End, FColor().Red, false, 5.0f, 0, 1);

		//コリジョン判定で無視する頂目を指定（今回はこのActor自分自身、thisポインタで指定)
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//ヒットしたオブジェクトを格納変数
		FHitResult OutHit;

		//コリジョン判定用
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);

		//ヒットするオブジェクトがある
		if (isHit)
		{
			//ヒットしたアクターのタグによって処理を変更する
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor"));
			//隠れる用オブジェクト
			if (OutHit.GetActor()->ActorHasTag("CanHide"))
			{
				AHideObjectBase* tmp = Cast<AHideObjectBase>(OutHit.GetActor());
				m_bHiding = m_bHiding ? false : true;
				tmp->SetHide(m_bHiding);
				//UE_LOG(LogTemp, Warning, TEXT("Hit Actor TagCanHide, isHide %d"),m_bHiding);
			}
			//プレイヤーアクション用
			if (OutHit.GetActor()->ActorHasTag("ActionPoint"))
			{
				ActionStart();
				PrimaryActorTick.SetTickFunctionEnable(true);
				//UE_LOG(LogTemp, Warning, TEXT("Action Start %d"));
			}
		}
		//何もなければ、ログを表示
		//else UE_LOG(LogTemp, Warning, TEXT("no Hit"));
	}
}
