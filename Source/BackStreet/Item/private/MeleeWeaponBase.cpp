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

	//근접 공격이 가능한 무기라면 근접 공격 로직 수행
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

	//hitResult가 Valid하다면 아래 조건문에서 데미지를 가함
	if (bIsMeleeTraceSucceed)
	{
		//효과를 출력
		ActivateMeleeHitEffect(hitResult.Location);

		//데미지를 주고, 중복 체크를 해준다.
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponMeleeDamage * WeaponStat.WeaponDamageRate
			, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);
		MeleeLineTraceQueryParams.AddIgnoredActor(hitResult.GetActor());

		//디버프도 부여
		if (IsValid(GamemodeRef->GetGlobalDebuffManagerRef()))
		{
			GamemodeRef->GetGlobalDebuffManagerRef()->SetDebuffTimer(WeaponStat.DebuffType, Cast<ACharacterBase>(hitResult.GetActor())
				, OwnerCharacterRef, WeaponStat.DebuffTotalTime, WeaponStat.DebuffVariable);
		}

		//내구도를 업데이트
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

			if (hitResult.bBlockingHit && IsValid(hitResult.GetActor()) //hitResult와 hitActor의 Validity 체크
				&& OwnerCharacterRef->Tags.IsValidIndex(1) //hitActor의 Tags 체크(1)
				&& hitResult.GetActor()->ActorHasTag("Character") //hitActor의 Type 체크
				&& !hitResult.GetActor()->ActorHasTag(OwnerCharacterRef->Tags[1])) //공격자와 피격자의 타입이 같은지 체크
			{
				return true;
			}
		}
	}
	return false;
}

void AMeleeWeaponBase::ActivateMeleeHitEffect(const FVector& Location)
{
	//근접 공격에서의 슬로우 효과를 시도
	TryActivateSlowHitEffect();

	//효과 이미터 출력
	if (IsValid(HitEffectParticle))
	{
		FTransform emitterSpawnTransform(FQuat(0.0f), Location, FVector(1.5f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);

	}

	//카메라 Shake 효과
	GamemodeRef->PlayCameraShakeEffect(OwnerCharacterRef->ActorHasTag("Player") ? ECameraShakeType::E_Attack : ECameraShakeType::E_Hit, Location);

	// 사운드
	if (HitImpactSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitImpactSound, GetActorLocation());
	}
}

bool AMeleeWeaponBase::TryActivateSlowHitEffect()
{
	//마지막 콤보 슬로우 효과, 추후 MaxComboCnt 프로퍼티 추가 예정
		//애니메이션 배열 소유는 캐릭터. OwnerCharacter->GetAnimArray(WeaponType).Num() ) 
		// ㄴ> 이런식으로 하면 될 거 같은데...
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