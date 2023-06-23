// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MeleeWeaponBase.h"
#include "../public/WeaponBase.h"
#include "../../Global/public/DebuffManager.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#define MAX_LINETRACE_POS_COUNT 6

void AMeleeWeaponBase::Attack()
{
	Super::Attack();

	//���� ������ ������ ������ ���� ���� ���� ����
	if (WeaponStat.bCanMeleeAtk)
	{
		PlayEffectSound(AttackSound);
		GetWorldTimerManager().SetTimer(MeleeAtkTimerHandle, this, &AMeleeWeaponBase::MeleeAttack, 0.01f, true);
		MeleeTrailParticle->Activate();
	}
	if (MeleeLineTraceQueryParams.GetIgnoredActors().Num() == 0)
	{
		MeleeLineTraceQueryParams.AddIgnoredActor(OwnerCharacterRef);
	}
}

void AMeleeWeaponBase::StopAttack()
{
	Super::StopAttack();

	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
	MeleePrevTracePointList.Empty();
	MeleeLineTraceQueryParams.ClearIgnoredActors();
	MeleeTrailParticle->Deactivate();
}

void AMeleeWeaponBase::ClearAllTimerHandle()
{
	Super::ClearAllTimerHandle();    
	MeleeTrailParticle->Deactivate();
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
	MeleePrevTracePointList.Empty(); 
	MeleeLineTraceQueryParams.ClearIgnoredActors();
}

void AMeleeWeaponBase::MeleeAttack()
{
	FHitResult hitResult;
	bool bIsMeleeTraceSucceed = false;

	TArray<FVector> currTracePositionList = GetCurrentMeleePointList();
	bIsMeleeTraceSucceed = CheckMeleeAttackTarget(hitResult, currTracePositionList);
	MeleePrevTracePointList = currTracePositionList;

	//hitResult�� Valid�ϴٸ� �Ʒ� ���ǹ����� �������� ����
	if (bIsMeleeTraceSucceed)
	{
		//ȿ���� ���
		ActivateMeleeHitEffect(hitResult.Location);

		//�������� �ְ�, �ߺ� üũ�� ���ش�.
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponMeleeDamage * WeaponStat.WeaponDamageRate
			, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);
		MeleeLineTraceQueryParams.AddIgnoredActor(hitResult.GetActor());

		//������� �ο�
		if (IsValid(GamemodeRef->GetGlobalDebuffManagerRef()))
		{
			GamemodeRef->GetGlobalDebuffManagerRef()->SetDebuffTimer(WeaponStat.DebuffType, Cast<ACharacterBase>(hitResult.GetActor())
				, OwnerCharacterRef, WeaponStat.DebuffTotalTime, WeaponStat.DebuffVariable);
		}

		//�������� ������Ʈ
		UpdateDurabilityState();
	}
}

bool AMeleeWeaponBase::CheckMeleeAttackTarget(FHitResult& hitResult, const TArray<FVector>& TracePositionList)
{
	if (MeleePrevTracePointList.Num() == MAX_LINETRACE_POS_COUNT)
	{
		for (uint8 tracePointIdx = 0; tracePointIdx < MAX_LINETRACE_POS_COUNT; tracePointIdx++)
		{
			const FVector& beginPoint = MeleePrevTracePointList[tracePointIdx];
			const FVector& endPoint = TracePositionList[tracePointIdx];
			GetWorld()->LineTraceSingleByChannel(hitResult, beginPoint, endPoint, ECollisionChannel::ECC_Camera, MeleeLineTraceQueryParams);

			if (hitResult.bBlockingHit && IsValid(hitResult.GetActor()) //hitResult�� hitActor�� Validity üũ
				&& OwnerCharacterRef->Tags.IsValidIndex(1) //hitActor�� Tags üũ(1)
				&& hitResult.GetActor()->ActorHasTag("Character") //hitActor�� Type üũ
				&& !hitResult.GetActor()->ActorHasTag(OwnerCharacterRef->Tags[1])) //�����ڿ� �ǰ����� Ÿ���� ������ üũ
			{
				return true;
			}
		}
	}
	return false;
}

void AMeleeWeaponBase::ActivateMeleeHitEffect(const FVector& Location)
{
	//���� ���ݿ����� ���ο� ȿ���� �õ�
	TryActivateSlowHitEffect();

	//ȿ�� �̹��� ���
	if (IsValid(HitEffectParticle))
	{
		FTransform emitterSpawnTransform(FQuat(0.0f), Location, FVector(1.5f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);

	}

	//ī�޶� Shake ȿ��
	GamemodeRef->PlayCameraShakeEffect(OwnerCharacterRef->ActorHasTag("Player") ? ECameraShakeType::E_Attack : ECameraShakeType::E_Hit, Location);

	// ����
	if (HitImpactSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitImpactSound, GetActorLocation());
	}
}

bool AMeleeWeaponBase::TryActivateSlowHitEffect()
{
	//������ �޺� ���ο� ȿ��, ���� MaxComboCnt ������Ƽ �߰� ����
		//�ִϸ��̼� �迭 ������ ĳ����. OwnerCharacter->GetAnimArray(WeaponType).Num() ) 
		// ��> �̷������� �ϸ� �� �� ������...
	if (OwnerCharacterRef->ActorHasTag("Player") && WeaponState.ComboCount % 4 == 0)
	{
		FTimerHandle attackSlowEffectTimerHandle;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
		GetWorldTimerManager().SetTimer(attackSlowEffectTimerHandle, FTimerDelegate::CreateLambda([&]() {
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			}), 0.015f, false);
		return true;
	}
	return false;
}

TArray<FVector> AMeleeWeaponBase::GetCurrentMeleePointList()
{
	TArray<FVector> resultPosList;

	resultPosList.Add(WeaponMesh->GetSocketLocation("GrabPoint"));
	resultPosList.Add(WeaponMesh->GetSocketLocation("End"));
	for (uint8 positionIdx = 1; positionIdx < MAX_LINETRACE_POS_COUNT - 1; positionIdx++)
	{
		FVector direction = resultPosList[1] - resultPosList[0];

		resultPosList.Add(resultPosList[0] + direction / MAX_LINETRACE_POS_COUNT * positionIdx);
	}
	return resultPosList;
}