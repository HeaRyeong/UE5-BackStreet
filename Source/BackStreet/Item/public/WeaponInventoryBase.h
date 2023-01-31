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

//----- 인벤토리 핵심 로직-------------------------------
public: 
	//인벤토리를 초기화
	UFUNCTION()
		void InitInventory();

	//무기 추가를 시도. 불가능하면 false를 반환
	UFUNCTION()
		bool AddWeapon(class AWeaponBase* NewWeapon);

	//무기를 인벤토리로부터 제거
	UFUNCTION()
		void RemoveWeapon(int32 InventoryIdx);

	//다음 무기로 전환
	UFUNCTION()
		bool SwitchToNextWeapon();

	//현재 무기의 정보와 인벤토리 내 정보를 동기화
	UFUNCTION(BlueprintCallable)
		void SyncCurrentWeaponInfo();

protected:
	UFUNCTION()
		class AWeaponBase* SpawnWeaponActor(int32 WeaponID);

	//현재 무기를 인벤토리로 보낸다.
	UFUNCTION()
		void RestoreCurrentWeapon();

	//인벤토리를 비움
	UFUNCTION()
		void ClearInventory();

	//인벤토리를 Weight를 기준으로 오름차순 정렬
	UFUNCTION()
		void SortInventory();

	//현재 장착하고 있는 Weapon Actor을 Destroy
	UFUNCTION()
		void DestroyCurrentWeapon();

//------ 프로퍼티 관련 ----------------------------------
public:
	//현재 인벤토리 Idx를 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentIdx() {  return CurrentIdx;  }

	//현재 무기 개수를 반환
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

	//현재 장비하고 있는 WeaponRef
	UPROPERTY()
		class AWeaponBase* CurrentWeaponRef;
};
