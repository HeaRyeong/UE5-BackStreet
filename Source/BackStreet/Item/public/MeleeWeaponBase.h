// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "../public/WeaponBase.h"
#include "MeleeWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AMeleeWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
public:
	//���� ó��
	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	//���� ������ ó��
	UFUNCTION(BlueprintCallable)
		virtual void StopAttack() override;

	UFUNCTION(BlueprintCallable)
		virtual float GetAttackRange() override;

protected:
	UFUNCTION()
		virtual void ClearAllTimerHandle() override;

//------ ����/���� ���� ---------------------------------
protected:
	UFUNCTION(BlueprintCallable)
		virtual void UpdateWeaponStat(FWeaponStatStruct NewStat) override;

//-------- Melee ���� ---------------------------
public:
	//���� ������ ����
	UFUNCTION()
		void MeleeAttack();
	
private:
	//�˷� Trace, ���� ������ �� �������� ���� ���� ��ǥ -> ���� ���� ��ǥ�� LineTrace�� ���� 
	UFUNCTION()
		bool CheckMeleeAttackTarget(FHitResult& hitResult, const TArray<FVector>& TracePositionList);

	//����, ��ƼŬ, ī�޶� ���� ���� ���� ȿ���� ����Ѵ�.
	UFUNCTION()
		void ActivateMeleeHitEffect(const FVector& Location);

	//������ �޺������� ���ο� Ÿ�� ȿ�� Ȱ��ȭ�� �õ��Ѵ�.
	UFUNCTION()
		bool TryActivateSlowHitEffect();

private:
	UFUNCTION()
		TArray<FVector> GetCurrentMeleePointList();

	UPROPERTY()
		TArray<FVector> MeleePrevTracePointList;

	//UPROPERTY()
	FCollisionQueryParams MeleeLineTraceQueryParams;

	UPROPERTY()
		FTimerHandle MeleeAtkTimerHandle;
};