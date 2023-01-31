// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/WeaponInventoryBase.h"
#include "../public/WeaponBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#define INVALID_INVENTORY_IDX -1

AWeaponInventoryBase::AWeaponInventoryBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
}

void AWeaponInventoryBase::Tick(float DeltaTime)
{
}

void AWeaponInventoryBase::BeginPlay()
{
}

void AWeaponInventoryBase::InitInventory()
{
	if (!IsValid(GetOwner()) || !GetOwner()->ActorHasTag("Character"))
	{
		UE_LOG(LogTemp, Warning, TEXT("AInventory는 CharacterBase 이외의 클래스에서는 소유할 수 없습니다."));
		return;
	}
	OwnerCharacterRef = Cast<ACharacterBase>(GetOwner());
	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());

	InventoryArray.Empty();
	for (int32 idx = 0; idx < MaxCapacity; idx++)
		InventoryArray.Add(FInventoryItemInfoStruct());
}

bool AWeaponInventoryBase::AddWeapon(AWeaponBase* NewWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("ADD #-1"));

	if (!IsValid(OwnerCharacterRef)) return false;

	UE_LOG(LogTemp, Warning, TEXT("ADD #0 / Capacity : %d,  Count : %d,  Idx : %d"), CurrentCapacity, CurrentWeaponCount, CurrentIdx);

	if (MaxCapacity < NewWeapon->WeaponStat.WeaponWeight + CurrentCapacity) return false;

	if (!WeaponClassInfoMap.Contains(NewWeapon->WeaponID))
	{
		WeaponClassInfoMap.Add(NewWeapon->WeaponID, NewWeapon->GetClass());
	}
	InventoryArray[GetNextInventoryIdx()] = {NewWeapon->WeaponID, NewWeapon->WeaponStat, NewWeapon->WeaponState};
	NewWeapon->Destroy();

	if (GetCurrentWeaponCount() == 0) //기존 인벤토리가 비어있었다면? 바로 장착까지 해줌
	{
		NewWeapon = SpawnWeaponActor(InventoryArray[GetNextInventoryIdx()].WeaponID);
		if (!IsValid(NewWeapon))
		{
			UE_LOG(LogTemp, Warning, TEXT("HOLY MOLY"));
			return false;
		}
		CurrentWeaponRef = NewWeapon;
		OwnerCharacterRef->EquipWeapon(NewWeapon);
	}
	UE_LOG(LogTemp, Warning, TEXT("ADD SUCCESSFUL"));

	CurrentCapacity += NewWeapon->WeaponStat.WeaponWeight;
	CurrentWeaponCount += 1;

	return true;
}

void AWeaponInventoryBase::RemoveWeapon(int32 InventoryIdx)
{
	SortInventory();
}

bool AWeaponInventoryBase::SwitchToNextWeapon()
{
	const int32 nextIdx = GetNextInventoryIdx();

	UE_LOG(LogTemp, Warning, TEXT("Switch #0 / Capacity : %d,  Count : %d,  Idx : %d"), CurrentCapacity, CurrentWeaponCount, CurrentIdx);

	if (GetCurrentWeaponCount() <= 1 || nextIdx == INVALID_INVENTORY_IDX) return false;
	if (!InventoryArray.IsValidIndex(nextIdx)) return false;
	if (!IsValid(OwnerCharacterRef)) return false;

	FInventoryItemInfoStruct& targetWeaponInfo = InventoryArray[nextIdx];

	RestoreCurrentWeapon(); //현재 무기를 Destroy하고, 정보만 List에 저장한다.

	AWeaponBase* newWeapon = SpawnWeaponActor(targetWeaponInfo.WeaponID);
	if(IsValid(newWeapon))
	{
		OwnerCharacterRef->EquipWeapon(newWeapon);
		CurrentWeaponRef = newWeapon;
		SetCurrentIdx(nextIdx);
		return true;
	}
	return false;
}

void AWeaponInventoryBase::SyncCurrentWeaponInfo()
{
	if (!IsValid(OwnerCharacterRef)) return;
	if (GetCurrentWeaponCount() == 0) return;
	if (InventoryArray.IsValidIndex(GetCurrentIdx()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SyncCurrentWeaponInfo : Invalid Index"));
		return;
	}

	AWeaponBase* currWeaponRef = OwnerCharacterRef->GetWeaponActorRef();
	if (IsValid(currWeaponRef))
	{
		UE_LOG(LogTemp, Warning, TEXT("SyncCurrentWeaponInfo : Weapon Not Found"));
		return;
	}
	InventoryArray[GetCurrentIdx()].WeaponStat = currWeaponRef->WeaponStat;
	InventoryArray[GetCurrentIdx()].WeaponState = currWeaponRef->WeaponState;
}

AWeaponBase* AWeaponInventoryBase::SpawnWeaponActor(int32 WeaponID)
{
	if (!WeaponClassInfoMap.Contains(WeaponID)) return nullptr;
	
	UClass* targetClass = *(WeaponClassInfoMap.Find(WeaponID));
	if (!targetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Class가 Invalid 합니다."));
		return nullptr;
	}
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = OwnerCharacterRef;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector spawnLocation = OwnerCharacterRef->GetActorLocation();
	FRotator spawnRotation = FRotator();
	AWeaponBase* newWeapon = Cast<AWeaponBase>(GetWorld()->SpawnActor(targetClass, &spawnLocation, &spawnRotation, spawnParams));

	return newWeapon;
}

void AWeaponInventoryBase::RestoreCurrentWeapon()
{
	if (GetCurrentWeaponCount() <= 1) return;
	if (!IsValid(OwnerCharacterRef)) return;
	
	SyncCurrentWeaponInfo();
	DestroyCurrentWeapon();
}

void AWeaponInventoryBase::ClearInventory()
{
	int32 searchIdx = 0;
	while (GetCurrentWeaponCount() > 0)
	{
		const int32 currWeight = InventoryArray[searchIdx].WeaponStat.WeaponWeight;
		InventoryArray[searchIdx] = FInventoryItemInfoStruct();
		searchIdx = (searchIdx + currWeight) % MaxCapacity;
		CurrentWeaponCount -= 1;
	}
}

void AWeaponInventoryBase::SortInventory()
{

}

void AWeaponInventoryBase::DestroyCurrentWeapon()
{
	if (!IsValid(GetCurrentWeaponRef())) return;
	CurrentWeaponRef->Destroy();
}

FInventoryItemInfoStruct AWeaponInventoryBase::GetCurrentWeaponInfo()
{
	if(GetCurrentWeaponCount() == 0) return FInventoryItemInfoStruct();
	return InventoryArray[GetCurrentIdx()];
}

int32 AWeaponInventoryBase::GetNextInventoryIdx()
{
	if (GetCurrentWeaponCount() == 0) return 0;
	const int32 currentWeaponWeight = (GetCurrentWeaponInfo().WeaponStat).WeaponWeight;
	return (GetCurrentIdx() + currentWeaponWeight) % MaxCapacity;
}
