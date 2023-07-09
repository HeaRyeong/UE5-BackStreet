// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "WeaponInventoryBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeleInventoryUpdate, const TArray<FInventoryItemInfoStruct>&, newInventoryInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDeleInventoryInfoUpdate, int32, inventoryIdx, bool, bIsCurrentWeapon, const FInventoryItemInfoStruct&, newInventoryInfo);

UCLASS()
class BACKSTREET_API AWeaponInventoryBase : public AActor
{
	GENERATED_BODY()
//------ Delegate ---------------------------------
public:
	//�κ��丮�� ������Ʈ �Ǿ��� �� (�߰�, ����, ���� ��)
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDeleInventoryUpdate OnInventoryIsUpdated;

	//�κ��丮 �� ���Ұ� ������Ʈ �Ǿ��� ��
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDeleInventoryInfoUpdate OnInventoryItemIsUpdated;

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

	//���⸦ ����
	UFUNCTION()
		void EquipWeapon(int32 InventoryIdx, bool bIsNewWeapon = false);

	//���� �߰��� �õ�. �Ұ����ϸ� false�� ��ȯ
	UFUNCTION()
		bool AddWeapon(int32 NewWeaponID);

	//���⸦ �κ��丮�κ��� ����
	UFUNCTION(BlueprintCallable)
		void RemoveWeapon(int32 WeaponID);

	//���� �����ϰ� �ִ� Weapon Actor ������ �����Ѵ�.
	UFUNCTION()
		void RemoveCurrentWeapon();

	//���� ����� ��ȯ
	UFUNCTION()
		bool SwitchToNextWeapon();

	//���� ������ ������ �κ��丮 �� ������ ����ȭ
	UFUNCTION(BlueprintCallable)
		void SyncCurrentWeaponInfo(bool bIsLoadInfo);

	//�ش� Weapon�� �κ��丮�� ������ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable)
		bool GetWeaponIsContained(int32 WeaponID);
	
	UFUNCTION(BlueprintCallable)
		bool TryAddAmmoToWeapon(int32 WeaponID, int32 AmmoCount);

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

	//�ش� Weapon�� �κ��丮�� ������ �Ǿ��ִ��� ��ȯ
	UFUNCTION()
		int32 GetWeaponInventoryIdx(int32 WeaponID);

	//�κ��丮�� TargetWeaponID��� ID�� ���� Weapon�� �ִ��� üũ.
	//�ߺ��Ǵ°� �ִٸ� �ش� �κ��丮�� Idx�� ��ȯ
	UFUNCTION()
		int32 CheckWeaponDuplicate(int32 TargetWeaponID);

//------ ������Ƽ ���� ----------------------------------
public:
	//���� ���õ� �κ��丮 Idx�� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentIdx() {  return CurrentIdx;  }

	//���� ���� ������ ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentWeaponCount() {  return CurrentWeaponCount = InventoryArray.Num(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentCapacity() { return CurrentCapacity; }

	UFUNCTION()
		class AWeaponBase* GetCurrentWeaponRef();

	UFUNCTION()
		FInventoryItemInfoStruct GetCurrentWeaponInfo();

private:
	UFUNCTION()
		int32 GetNextInventoryIdx();

	UFUNCTION()
		void SetCurrentWeaponRef(class AWeaponBase* NewWeapon);

	UFUNCTION()
		void SetCurrentIdx(int32 newValue) { CurrentIdx = newValue; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (UIMin = 1, UIMax = 10))
		int32 MaxCapacity = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (UIMin = 1, UIMax = 10))
		int32 MaxWeaponCount = 6;

	//Weapon ID �迭, BP���� �����ϰ� Idx�� ����
	UPROPERTY(EditAnywhere, Category = "Gameplay|Class")
		TArray<int32> WeaponIDList;

	//Weapon Class �迭, BP���� �����ϰ� Idx�� ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Class")
		TArray<TSubclassOf<class AWeaponBase> > WeaponClassList;


private:
	UPROPERTY()
		TArray<FInventoryItemInfoStruct> InventoryArray;

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
