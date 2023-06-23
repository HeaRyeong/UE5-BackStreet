#pragma once
#include "BackStreet.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "AssetManagerBase.generated.h"
#define MAX_STAGE_CNT 5

//ĳ������ �ִϸ��̼� ���� ��θ� �����ϴ� ������ ���̺� ����ü
USTRUCT(BlueprintType)
struct FCharacterAnimAssetInfoStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	//ĳ������ ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 CharacterID;

	//���� ���� �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> MeleeAttackAnimMontageList;

	//��� �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> ShootAnimMontageList;

	//��ô ���� �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> ThrowAnimMontageList;

	//������ �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> ReloadAnimMontageList;

	//Ÿ�� �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> HitAnimMontageList;

	//������ �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> RollAnimMontageList;

	//��ȣ�ۿ� �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> InvestigateAnimMontageList;

	//��� �ִϸ��̼� / List�� ���� -> �����ϰ� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> DieAnimMontageList;
};

//ĳ������ VFX ���� ��θ� �����ϴ� ������ ���̺� ����ü
USTRUCT(BlueprintType)
struct FCharacterVFXAssetInfoStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	//ĳ������ ����� ���̾ư��� ����Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<class UNiagaraSystem*> DebuffNiagaraEffectLis;
};

//ĳ������ ���� ��θ� �����ϴ� ������ ���̺� ����ü
USTRUCT(BlueprintType)
struct FCharacterAssetInfoStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//ĳ���� �ִϸ��̼� ���� ��� ����
	UPROPERTY()
		TMap<int32, FCharacterAnimAssetInfoStruct> AnimAssetInfoMap;

	//ĳ���� VFX ���� ��� ����
	UPROPERTY()
		TMap<int32, FCharacterVFXAssetInfoStruct> VFXAssetInfoMap;
};

UCLASS()
class BACKSTREET_API AAssetManagerBase : public AActor
{
	GENERATED_BODY()

public:
	AAssetManagerBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		TSubclassOf<AEnemyCharacterBase> GetEnemyWithID(int32 EnemyID);

	UFUNCTION()
		ULevelSequence* GetTileTravelEffectSequence() { return TileTravelEffectSequence; }

	UFUNCTION()
		ULevelSequence* GetFadeOutEffectSequence() { return FadeOutEffectSequence; }

	UFUNCTION()
		ULevelSequence* GetFadeInEffectSequence() { return FadeInEffectSequence; }

	UFUNCTION()
		FName GetRandomMap();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AEnemyCharacterBase>> EnemyAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AEnemyCharacterBase>> MissionEnemyAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AItemBoxBase>> ItemBoxAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AWeaponBase>> WeaponAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class ARewardBoxBase>> RewardBoxAssets;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AProjectileBase>> ProjectileAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AItemBoxBase>> MissionAssets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|VFX")
		class ULevelSequence* TileTravelEffectSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|VFX")
		class ULevelSequence* FadeOutEffectSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|VFX")
		class ULevelSequence* FadeInEffectSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> MapNames;

private:
	UPROPERTY()
		class ABackStreetGameModeBase* GameModeRef;

};

