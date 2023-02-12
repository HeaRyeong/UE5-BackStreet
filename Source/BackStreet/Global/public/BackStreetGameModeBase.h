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
	UFUNCTION()
		void StartGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMiniMapUI();

public:
	UPROPERTY()
		class AChapterManagerBase* ChapterManager;


// Asset

public:
	UFUNCTION()
		void CreateAssetManager();

	UFUNCTION(BlueprintCallable)
		AAssetManagerBase* GetAssetManager() { return AssetManager; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AAssetManagerBase> AssetManagerBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAssetManagerBase* AssetManager;


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
		void UpdateCharacterStatWithID(class ACharacterBase* TargetCharacter, const uint32 CharacterID);

	UFUNCTION()
		void UpdateWeaponStat(class AWeaponBase* TargetWeapon, FWeaponStatStruct NewStat);

	UFUNCTION()
		void UpdateWeaponStatWithID(class AWeaponBase* TargetWeapon, const uint8 WeaponID);

	UFUNCTION()
		void UpdateProjectileStatWithID(class AProjectileBase* TargetProjectile, const uint8 ProjectileID);

	UFUNCTION()
		FWeaponStatStruct GetWeaponStatInfoWithID(const uint8 WeaponID);

	UFUNCTION()
		FStageEnemyTypeStruct GetStageTypeInfoWithRow(uint16 rowName);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* StageTypeTable;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		TArray<TSubclassOf<UCameraShakeBase> > CameraShakeEffectList;

	UPROPERTY(BlueprintReadWrite)
		bool bIsGamePaused = false;

//------ Ref 멤버 ---------------
private:
	UPROPERTY()
		class AMainCharacterBase* PlayerCharacterRef;
};
