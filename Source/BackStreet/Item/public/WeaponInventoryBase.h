// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "WeaponInventoryBase.generated.h"


UCLASS()
class BACKSTREET_API AWeaponInventoryBase : public AActor
{
	GENERATED_BODY()

//------ Global ------------------------------------
public:
	AWeaponInventoryBase();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//----- �κ��丮 �ٽ� ����-------------------------------
public: 
	//�κ��丮�� �ʱ�ȭ
	UFUNCTION()
		void InitInventory();

	//���� �߰��� �õ�. �Ұ����ϸ� false�� ��ȯ
	UFUNCTION()
		bool AddWeapon(class AWeaponBase* NewWeapon);

	//���⸦ �κ��丮�κ��� ����
	UFUNCTION()
		void RemoveWeapon(int32 InventoryIdx);

	//���� ����� ��ȯ
	UFUNCTION()
		bool SwitchToNextWeapon();

	//���� ������ ������ �κ��丮 �� ������ ����ȭ
	UFUNCTION(BlueprintCallable)
		void SyncCurrentWeaponInfo();

protected:
	UFUNCTION()
		class AWeaponBase* SpawnWeaponActor(int32 WeaponID);

	//���� ���⸦ �κ��丮�� ������.
	UFUNCTION()
		void RestoreCurrentWeapon();

	//�κ��丮�� ���
	UFUNCTION()
		void ClearInventory();

	//�κ��丮�� Weight�� �������� �������� ����
	UFUNCTION()
		void SortInventory();

	//���� �����ϰ� �ִ� Weapon Actor�� Destroy
	UFUNCTION()
		void DestroyCurrentWeapon();

//------ ������Ƽ ���� ----------------------------------
public:
	//���� �κ��丮 Idx�� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentIdx() {  return CurrentIdx;  }

	//���� ���� ������ ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentWeaponCount() {  return CurrentWeaponCount;  }

	UFUNCTION()
		class AWeaponBase* GetCurrentWeaponRef() { return CurrentWeaponRef; }

	UFUNCTION()
		FInventoryItemInfoStruct GetCurrentWeaponInfo();

private:
	UFUNCTION()
		int32 GetNextInventoryIdx();

	UFUNCTION()
		void SetCurrentIdx(int32 newValue) { CurrentIdx = newValue; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		int32 MaxCapacity = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		int32 MaxWeaponCount = 6;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|InventoryState")
		TArray<FInventoryItemInfoStruct> InventoryArray;

private:
	UPROPERTY()
		int32 CurrentIdx = 0; 

	UPROPERTY()
		int32 CurrentCapacity = 0;

	UPROPERTY()
		int32 CurrentWeaponCount = 0;

	UPROPERTY()
		TMap<int32, UClass*> WeaponClassInfoMap;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	//���� ����ϰ� �ִ� WeaponRef
	UPROPERTY()
		class AWeaponBase* CurrentWeaponRef;
};
