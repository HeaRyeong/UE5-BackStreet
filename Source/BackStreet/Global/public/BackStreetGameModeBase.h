// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../public/BackStreet.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/StreamableManager.h"
#include "BackStreetGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateNoParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSingleParam, bool, bGameIsOver);

UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateNoParam StartChapterDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateSingleParam FinishChapterDelegate;
	
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateNoParam ClearResourceDelegate;

public:
	ABackStreetGameModeBase();

protected:
	virtual void BeginPlay() override;

//---------- StageManager? -------------------------------------
public:
	UFUNCTION(BlueprintCallable)
		void InitializeChapter(); //@ljh 아래 InitChapter과 겹치네요..

	UFUNCTION(BlueprintCallable)
		void InitChapter();

	UFUNCTION(BlueprintCallable)
		void NextStage(uint8 Dir);

	UFUNCTION(BlueprintCallable)
		void MoveTile(uint8 NextDir);

	UFUNCTION(BlueprintCallable)
		void ClearChapter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AGridBase* Chapter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* CurrentTile;

	// 남은 챕터 수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RemainChapter;

	// 다음 스테이지 기준으로 입구 게이트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 PreDir = (uint8)(EDirection::E_DOWN);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ChapterStatValue;

//----- Asset관련--------------------------------------
public:
	UFUNCTION()
		void CreateAssetManager();

	UFUNCTION(BlueprintCallable)
		AAssetManagerBase* GetAssetManager();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AAssetManagerBase* AssetDataManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FSoftObjectPath AssetManagerBPPath;

	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	FStreamableManager StreamableManager = FStreamableManager();

	UPROPERTY(BlueprintReadWrite)
		bool bIsGamePaused = false;

// ----- Gameplay Manager -------------------
public:
	UFUNCTION()
		void StartChapter();

	UFUNCTION(BlueprintImplementableEvent)
		void FinishChapter(bool bGameIsOver);

	UFUNCTION(BlueprintCallable)
		void RewardStageClear(EStatUpCategoryInfo RewardType);

	UFUNCTION()
		void PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius = 100.0f);

	UFUNCTION()
		void UpdateCharacterStat(class ACharacterBase* TargetCharacter, FCharacterStatStruct NewStat);

	//UREFLECTION은 함수 오버로딩 미지원
	UFUNCTION()
		void UpdateCharacterStatWithID(class ACharacterBase* TargetCharacter, const uint8 CharacterID);

	UFUNCTION()
		void UpdateWeaponStat(class AWeaponBase* TargetWeapon, FWeaponStatStruct NewStat);

	UFUNCTION()
		void UpdateWeaponStatWithID(class AWeaponBase* TargetWeapon, const uint8 WeaponID);

	UFUNCTION()
		void UpdateProjectileStatWithID(class AProjectileBase* TargetProjectile, const uint8 ProjectileID);

	UFUNCTION()
		FWeaponStatStruct GetWeaponStatInfoWithID(const uint8 WeaponID);

protected:
	//적의 스탯 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* EnemyStatTable;
	
	//무기 스탯 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* WeaponStatTable;

	//발사체 스탯 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* ProjectileStatTable;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		TArray<TSubclassOf<UCameraShakeBase> > CameraShakeEffectList;

//------ Ref 멤버 ---------------
private:
	UPROPERTY()
		class AMainCharacterBase* PlayerCharacterRef;
};
