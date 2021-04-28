//-------------------------------------------------------------------
//�N���X���FHideObjectBase.h
//�T�v	�@�F�B���p�A�C�e���̃N���X
//�쐬�ҁ@�F19CU0228 ��c���_
//�쐬���@�F2020/11/27
//�X�V�@�@�F2020/11/27�@�N���X�쐬
//-------------------------------------------------------------------

//�C���N���[�h�J�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HideObjectBase.generated.h"


class UBoxComponent;
class USkeltalComponent;
class UStaticMeshComponent;
class UStaticMesh;

UCLASS()
class ESPIONAGE_API AHideObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	//�R���X�g���N�^
	AHideObjectBase();

protected:
	//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:	
	//���t���[���X�V����
	virtual void Tick(float DeltaTime) override;


private:
	void HideObject(float _deltaTime);

public:
	void SetHide(bool _isLift);

	bool GetHide()const { return m_isHide; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pStaticMeshComp;
	UPROPERTY(EditAnywhere, Category = "Player Character")
		AActor* m_pPlayerChara;

protected:
	UPROPERTY()
		USceneComponent* m_pBase;

private:
	UStaticMesh* m_pStaticMesh;
	
	FVector m_MeshSize;

	bool m_isHide;

};
