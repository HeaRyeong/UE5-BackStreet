// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "../public/WeaponBase.h"
#include "RangedWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API ARangedWeaponBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	//���� ó��
	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	//���� ������ ó��
	UFUNCTION(BlueprintCallable)
		virtual void StopAttack() override;

protected:
	UFUNCTION()
		virtual void ClearAllTimerHandle() override;

//------ Projectile ����----------------------------
public:
	//�߻�ü�� ����
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	//�߻�ü �ʱ�ȭ �� �߻縦 �õ� 
	UFUNCTION()
		bool TryFireProjectile();

	//������ �õ�. ���� ���¿� ���� ���� ���θ� ��ȯ
	UFUNCTION(BlueprintCallable)
		bool TryReload();

	//���� źȯ�� ������ ��ȯ - Stat.TotalAmmoCount
	UFUNCTION(BlueprintCallable)
		int32 GetLeftAmmoCount() { return WeaponState.TotalAmmoCount; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetCanReload();

	//źȯ�� ������ ���� (TotalAmmoCount����)
	UFUNCTION(BlueprintCallable)
		void AddAmmo(int32 Count);

	//źâ�� ������ ���� (+= MaxAmmoPerMagazine*Count)
	UFUNCTION()
		void AddMagazine(int32 Count);

	UFUNCTION()
		void SetInfiniteAmmoMode(bool NewMode) { WeaponStat.bIsInfiniteAmmo = NewMode; }

protected:
	//SoftObjRef�� ��ü ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

private:
	UPROPERTY()
		FTimerHandle AutoReloadTimerHandle;
};
