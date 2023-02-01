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

	//���⸦ ����
	UFUNCTION()
		void EquipWeapon(int32 InventoryIdx);

	//���� �߰��� �õ�. �Ұ����ϸ� false�� ��ȯ
	UFUNCTION()
		bool AddWeapon(int32 NewWeaponID);

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

	//���� �����ϰ� �ִ� Weapon Actor ������ �����Ѵ�.
	UFUNCTION()
		void RemoveCurrentWeapon();

	//�κ��丮�� TargetWeaponID��� ID�� ���� Weapon�� �ִ��� üũ.
	//�ߺ��Ǵ°� �ִٸ� �ش� �κ��丮�� Idx�� ��ȯ
	UFUNCTION()
		int32 CheckWeaponDuplicate(int32 TargetWeaponID);

//------ ������Ƽ ���� ----------------------------------
public:
	//���� �κ��丮 Idx�� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentIdx() {  return CurrentIdx;  }

	//���� ���� ������ ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentWeaponCount() {  return CurrentWeaponCount = InventoryArray.Num(); }

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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Class")
		TArray<TSubclassOf<class AWeaponBase> > WeaponClassList;

	UPROPERTY(EditAnywhere, Category = "Gameplay|Class")
		TArray<int32> WeaponIDList;

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
