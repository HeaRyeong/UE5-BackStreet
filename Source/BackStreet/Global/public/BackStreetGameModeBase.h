// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../public/BackStreet.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/StreamableManager.h"
#include "BackStreetGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateClearResource);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSingleParam, bool, bGameIsOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateSystemMessage, FName, Message, FColor, TextColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateStageClear);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateMissionClear);

UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateSingleParam StartChapterDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateSingleParam FinishChapterDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateClearResource ClearResourceDelegate;
	
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateClearResource ChapterClearResourceDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateSystemMessage PrintSystemMessageDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateStageClear StageClearDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateStageClear FadeOutDelegate;


public:
	ABackStreetGameModeBase();

protected:
	virtual void BeginPlay() override;


// ----- Gameplay Manager -------------------
public:
	UFUNCTION(BlueprintImplementableEvent)
		void PrintDebugMessage();

	UFUNCTION(BlueprintImplementableEvent)
		void FinishChapter(bool bGameIsOver);

	UFUNCTION(BlueprintCallable)
		void PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius = 100.0f);

	UFUNCTION()
		AItemBase* SpawnItemToWorld(uint8 ItemType, uint8 ItemID, FVector SpawnLocation);

	UFUNCTION()
		void UpdateCharacterStat(class ACharacterBase* TargetCharacter, FCharacterStatStruct NewStat);

	UFUNCTION()
		void UpdateCharacterStatWithID(class ACharacterBase* TargetCharacter, const uint32 CharacterID);

	UFUNCTION()
		void UpdateWeaponStat(class AWeaponBase* TargetWeapon, FWeaponStatStruct NewStat);

	UFUNCTION()
		void UpdateWeaponStatWithID(class AWeaponBase* TargetWeapon, const uint8 WeaponID);

	UFUNCTION()
		void UpdateProjectileStatWithID(class AProjectileBase* TargetProjectile, const uint8 ProjectileID);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FWeaponStatStruct GetWeaponStatInfoWithID(const uint8 WeaponID);

	UFUNCTION()
		FStageEnemyTypeStruct GetStageTypeInfoWithRow(uint16 rowName);

	UFUNCTION()
		class UDebuffManager* GetGlobalDebuffManagerRef() { return DebuffManager; }

	// ------ Asset Info ----------------------------
public:
	//ĳ������ �ִϸ��̼� ���� �����͸� �о����
	UFUNCTION()
		FCharacterAnimAssetInfoStruct GetCharacterAnimAssetInfoData(const int32 CharacterID);

protected:
	//�ִϸ��̼� ���� ��� ���� ������ ���̺�
	UPROPERTY(EditDefaultsOnly, Category = "Asset|Data")
		UDataTable* AnimAssetInfoTable;

	//VFX ���� ��� ���� ������ ���̺�
	UPROPERTY(EditDefaultsOnly, Category = "Asset|Data")
		UDataTable* VFXssetInfoTable;

private:
	//���� ���� �� ����
	UPROPERTY()
		FCharacterAssetInfoStruct CachedCharacterAssetInfoData;

	UFUNCTION(BlueprintCallable)
		class AChapterManagerBase* GetChapterManagerRef() { return ChapterManager; }

	UFUNCTION(BlueprintImplementableEvent)
		void FadeOut();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMiniMapUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMiniMapUI();

	UFUNCTION(BlueprintImplementableEvent)
		void PopUpClearUI();


// ------ Data Table -----------------------------
protected:
	//���� ���� ���̺�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* EnemyStatTable;
	
	//���� ���� ���̺�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* WeaponStatTable;

	//�߻�ü ���� ���̺�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* ProjectileStatTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* StageTypeTable;

// ----- Class Info ------------------------------------ 
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		TArray<TSubclassOf<UCameraShakeBase> > CameraShakeEffectList;
	
	//EItemCategoryInfo�� ���� Idx
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Item")
		TMap<int32, TSubclassOf<class AItemBase> > ItemClassMap;

//------ �� �� ������Ƽ ---------------
private:
	UPROPERTY()
		class AMainCharacterBase* PlayerCharacterRef;

	UPROPERTY()
		class UDebuffManager* DebuffManager;

	UPROPERTY()
		class AChapterManagerBase* ChapterManager;

public:
	//���� ���� ��尡 �ΰ������� Ʈ���������� Ȯ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsInGame;

protected: 
	//���� �Ͻ����� ����
	UPROPERTY(BlueprintReadWrite)
		bool bIsGamePaused = false;


};
