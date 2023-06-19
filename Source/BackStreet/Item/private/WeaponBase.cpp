// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/WeaponBase.h"
#include "Components/AudioComponent.h"
#include "../../Global/public/DebuffManager.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON_MESH"));
	WeaponMesh->SetupAttachment(DefaultSceneRoot);

	MeleeTrailParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ITEM_NIAGARA_COMPONENT"));
	MeleeTrailParticle->SetupAttachment(WeaponMesh);
	MeleeTrailParticle->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	InitWeapon();
}

void AWeaponBase::InitWeapon()
{
	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//무기 스탯이 초기화가 되지 않았다면
	if (WeaponStat.WeaponName == FName(""))
	{
		GamemodeRef->UpdateWeaponStatWithID(this, WeaponID);
	}
	WeaponState.CurrentDurability = WeaponStat.MaxDurability;
}

void AWeaponBase::RevertWeaponInfo(FWeaponStatStruct OldWeaponStat, FWeaponStateStruct OldWeaponState)
{
	if (OldWeaponStat.WeaponName == FName("")) return;
	WeaponStat = OldWeaponStat;
	WeaponState = OldWeaponState;
}

void AWeaponBase::Attack()
{
	WeaponState.ComboCount = (WeaponState.ComboCount + 1); //UpdateComboState()? 
}

void AWeaponBase::StopAttack()
{
}

void AWeaponBase::UpdateWeaponStat(FWeaponStatStruct NewStat)
{
	if (NewStat.MaxDurability == 0) return;
	WeaponStat = NewStat;
}

void AWeaponBase::SetResetComboTimer()
{
	GetWorldTimerManager().ClearTimer(ComboTimerHandle);
	GetWorldTimerManager().SetTimer(ComboTimerHandle, FTimerDelegate::CreateLambda([&]() {
		WeaponState.ComboCount = 0;
	}), 0.75f, false);
}

void AWeaponBase::SetOwnerCharacter(ACharacterBase* NewOwnerCharacterRef)
{
	if (!IsValid(NewOwnerCharacterRef)) return;
	OwnerCharacterRef = NewOwnerCharacterRef;
}

void AWeaponBase::PlayEffectSound(USoundCue* EffectSound)
{
	if (EffectSound == nullptr || !IsValid(OwnerCharacterRef) || !OwnerCharacterRef->ActorHasTag("Player")) return;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectSound, GetActorLocation());
}

void AWeaponBase::ClearAllTimerHandle()
{
	//* 자식 클래스에서 오버라이드 *//
}

float AWeaponBase::GetAttackRange()
{
	if (!IsValid(OwnerCharacterRef)) return 200.0f;

	if (WeaponStat.WeaponType == EWeaponType::E_Melee ||
		(!OwnerCharacterRef->GetCharacterStat().bInfinite && WeaponState.CurrentAmmoCount == 0.0f && WeaponState.TotalAmmoCount == 0.0f))
	{
		return 150.0f; //매크로나 const 멤버로 수정하기 
	}
	return 700.0f;
}

void AWeaponBase::UpdateDurabilityState()
{
	if (OwnerCharacterRef->GetCharacterStat().bInfinite || WeaponStat.bInfinite) return;
	if (--WeaponState.CurrentDurability == 0)
	{
		if (IsValid(DestroyEffectParticle))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffectParticle, GetActorLocation(), FRotator()); //파괴 효과
		}
		ClearAllTimerHandle();
		OwnerCharacterRef->StopAttack();
		WeaponDestroyDelegate.ExecuteIfBound();
	}
}

