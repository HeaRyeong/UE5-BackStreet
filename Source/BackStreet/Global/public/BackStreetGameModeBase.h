// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../public/BackStreet.h"
#include "../../StageSystem/public/DirectionEnumInfo.h"
#include "GameFramework/GameModeBase.h"
#include "AssetManagerBase.h"
#include "BackStreetGameModeBase.generated.h"


UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABackStreetGameModeBase();

public:
	UFUNCTION(BlueprintCallable)
		void InitializeChapter();

// StageManager? ----
public:
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
		class ATileBase* CurrTile;
	// ���� é�� �� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RemainChapter;

	// ���� �������� �������� �Ա� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 PreDir = (uint8)(EDirection::E_DOWN);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ChapterStatValue;

//-- Asset����-------------------
public:
	UPROPERTY(EditAnywhere)
		class UAssetManagerBase* AssetDataManager;

//	UPROPERTY(EditAnywhere)
		FStreamableManager StreamableManager;

	UPROPERTY(BlueprintReadWrite)
		bool bIsGamePaused = false;

//Gameplay Manager
public:
	UFUNCTION()
		void PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius = 100.0f);

	UFUNCTION()
		void UpdateCharacterStat(class ACharacterBase* TargetCharacter, FCharacterStatStruct NewStat);

	//UREFLECTION�� �Լ� �����ε� ������
	UFUNCTION()
		void UpdateCharacterStatWithID(class ACharacterBase* TargetCharacter, const uint8 CharacterID);

	UFUNCTION()
		void UpdateWeaponStat(class AWeaponBase* TargetWeapon, FWeaponStatStruct NewStat);

	UFUNCTION()
		void UpdateWeaponStatWithID(class AWeaponBase* TargetWeapon, const uint8 WeaponID);

	UFUNCTION()
		void UpdateProjectileStatWithID(class AProjectileBase* TargetProjectile, const uint8 ProjectileID);

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

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		TArray<TSubclassOf<UCameraShakeBase> > CameraShakeEffectList;
};
