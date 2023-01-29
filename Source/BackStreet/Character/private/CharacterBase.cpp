// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/CharacterBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Animation/AnimMontage.h"

#define MAX_BUFF_IDX 6
#define MAX_DEBUFF_IDX 7
#define HEAL_BUFF_TIMER_IDX 6
#define DEBUFF_DAMAGE_TIMER_IDX 15

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("WEAPON"));
	WeaponActor->SetupAttachment(GetMesh(), FName("Weapon_R"));

	for (int newTimerIdx = 0; newTimerIdx < 18; newTimerIdx += 1)
	{
		BuffDebuffTimerHandleList.Add(FTimerHandle());
	}
	this->Tags.Add("Character");
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitGamemodeRef();
	InitWeapon();
	InitCharacterState();
}

void ACharacterBase::InitCharacterState()
{
	GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
	CharacterState.CharacterCurrHP = CharacterStat.CharacterMaxHP;
	CharacterState.bCanAttack = true;
	CharacterState.CharacterActionState = ECharacterActionType::E_Idle;
}

void ACharacterBase::UpdateCharacterStat(FCharacterStatStruct NewStat)
{
	CharacterStat = NewStat;
	GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
}

void ACharacterBase::ResetActionState()
{
	CharacterState.CharacterActionState = ECharacterActionType::E_Idle;
	if (!GetWorldTimerManager().IsTimerActive(AtkIntervalHandle))
	{
		CharacterState.bCanAttack = true;
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator
								, AActor* DamageCauser)
{
	DamageAmount = DamageAmount - DamageAmount * CharacterStat.CharacterDefense;
	if (DamageAmount <= 0.0f || !IsValid(DamageCauser)) return 0.0f;
	if (CharacterStat.bIsInvincibility) return 0.0f;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterState.CharacterCurrHP = CharacterState.CharacterCurrHP - DamageAmount;
	CharacterState.CharacterCurrHP = FMath::Max(0.0f, CharacterState.CharacterCurrHP);
	if (CharacterState.CharacterCurrHP == 0.0f)
	{
		Die();
	}
	else if (IsValid(HitAnimMontage))
	{
		PlayAnimMontage(HitAnimMontage);
	}
	return DamageAmount;
}

float ACharacterBase::TakeDebuffDamage(float DamageAmount, uint8 DebuffType, AActor* Causer)
{
	//추후 디버프 당 파티클 시스템 추가 예정
	if (!IsValid(Causer)) return 0.0f;
	TakeDamage(DamageAmount, FDamageEvent(), nullptr, Causer);

	if (BuffRemainingTime[1][(int)DebuffType] <= 0.0f)
	{
		ClearBuffTimer(1, DebuffType);
	}
	return DamageAmount;
}

void ACharacterBase::TakeHeal(float HealAmount, bool bIsTimerEvent, uint8 BuffType)
{
	CharacterState.CharacterCurrHP += HealAmount;
	CharacterState.CharacterCurrHP = FMath::Min(CharacterStat.CharacterMaxHP, CharacterState.CharacterCurrHP);
	return;
}
}

void ACharacterBase::Die()
{
	if (!IsValid(DieAnimMontage)) return;

	CharacterState.CharacterActionState = ECharacterActionType::E_Die;
	CharacterStat.bIsInvincibility = true;
	ClearAllBuffTimer(true);
	ClearAllBuffTimer(false);
	ClearAllTimerHandle();
	
	PlayAnimMontage(DieAnimMontage);
	GetCharacterMovement()->Deactivate();
	bUseControllerRotationYaw = false;

	if (this->ActorHasTag(FName("Enemy")))
	{
		UE_LOG(LogTemp, Log, TEXT("Actor Has Tag Enemy"));
		if (FDieDelegate.IsBound())
		{
			FDieDelegate.Execute(this);
			FDieDelegate.Unbind();
}
	}


	GetWorldTimerManager().SetTimer(ReloadTimerHandle, FTimerDelegate::CreateLambda([&]() {
		//GameModeRef->SpawnItemOnLocation(GetActorLocation(), ItemID);
		Destroy();
	}), 1.0f, false, DieAnimMontage->GetPlayLength());
}

void ACharacterBase::TryAttack()
{
	if (AttackAnimMontageArray.Num() <= 0) return;
	if (!IsValid(WeaponActor->GetChildActor())) return;
	if (GetWorldTimerManager().IsTimerActive(AtkIntervalHandle)) return;
	if (!CharacterState.bCanAttack || !GetIsActionActive(ECharacterActionType::E_Idle)) return;

	AWeaponBase* weaponRef = Cast<AWeaponBase>(WeaponActor->GetChildActor());
	if (IsValid(weaponRef))
	{
		CharacterState.bCanAttack = false; //공격간 Delay,Interval 조절을 위해 세팅
		CharacterState.CharacterActionState = ECharacterActionType::E_Attack;

		const int32 nextAnimIdx = weaponRef->GetCurrentComboCnt() % AttackAnimMontageArray.Num();
		PlayAnimMontage(AttackAnimMontageArray[nextAnimIdx]);
	}
}

void ACharacterBase::Attack()
{
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
	GetWorldTimerManager().SetTimer(AtkIntervalHandle, this, &ACharacterBase::ResetAtkIntervalTimer
										, 1.0f, false, FMath::Max(0.0f, 1.0f - CharacterStat.CharacterAtkSpeed));

	AWeaponBase* weaponRef = Cast<AWeaponBase>(WeaponActor->GetChildActor());
	if (IsValid(weaponRef))
	{
		weaponRef->Attack();
	}
}
 
void ACharacterBase::StopAttack()
{
	ResetActionState();
	AWeaponBase* weaponRef = Cast<AWeaponBase>(WeaponActor->GetChildActor());
	if (IsValid(weaponRef))
	{
		weaponRef->StopAttack();
	}
}

void ACharacterBase::TryReload()
{
	if (IsValid(GetWeaponActorRef()))
	{
		if (!GetWeaponActorRef()->GetCanReload())
		{
			UE_LOG(LogTemp, Warning, TEXT("CAN'T RELOAD"));
			return;
		}

		float reloadTime = 0.75f;
		if (IsValid(ReloadAnimMontage)) reloadTime = PlayAnimMontage(ReloadAnimMontage) / 2.0f;

		CharacterState.CharacterActionState = ECharacterActionType::E_Reload;
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, FTimerDelegate::CreateLambda([&](){
			GetWeaponActorRef()->TryReload();
			ResetActionState();
		}), 1.0f, false, reloadTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MELEE_WEAPON CAN'T RELOAD"));
		return;
	}
}

void ACharacterBase::ResetAtkIntervalTimer()
{
	CharacterState.bCanAttack = true;
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
}

void ACharacterBase::InitWeapon()
{
	if (IsValid(GetWeaponActorRef()))
	{
		GetWeaponActorRef()->InitOwnerCharacterRef(this);
		WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Weapon_R");
		WeaponActor->SetRelativeLocation(FVector(0.0f), false);
	}
}

AWeaponBase* ACharacterBase::GetWeaponActorRef()
{
	if (IsValid(WeaponActor->GetChildActor()))
	{
		return Cast<AWeaponBase>(WeaponActor->GetChildActor());
	}
	return nullptr;
}

bool ACharacterBase::SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime, float Variable)
{
	FTimerDelegate TimerDelegate;
	FTimerHandle& timerHandle = GetBuffTimerHandleRef(bIsDebuff, BuffType);

	if ((bIsDebuff && GetDebuffIsActive((ECharacterDebuffType)BuffType))
		|| (!bIsDebuff && GetBuffIsActive((ECharacterBuffType)BuffType)))
	{
		const float newTime = GetWorldTimerManager().GetTimerRemaining(timerHandle) + TotalTime;
		GetWorldTimerManager().SetTimer(timerHandle, 1.0f, false, newTime);
		return true;
	}
	if (GetWorldTimerManager().IsTimerActive(timerHandle)) return;

	BuffRemainingTime[bIsDebuff][(int)BuffType] = TotalTime;

	/*---- 디버프 타이머 세팅 ----------------------------*/
	if (bIsDebuff)
	{
		Variable = FMath::Min(1.0f, FMath::Abs(Variable)); //값 정제
		CharacterState.CharacterDebuffState |= (1 << (int)BuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)

		switch ((ECharacterDebuffType)BuffType)
		{
			//----데미지 디버프-------------------
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			//SpawnBuffParticle(E_Flame, TotalTime) //TimerDelegate에 걸어도 됨
			TimerDelegate.BindUFunction(this, FName("TakeDebuffDamage"), Variable, BuffType, Causer);
			GetWorldTimerManager().SetTimer(BuffDebuffTimerHandleList[DEBUFF_DAMAGE_TIMER_IDX], TimerDelegate, 1.0f, true);
			break;
			//----스탯 조정 디버프-------------------
		case ECharacterDebuffType::E_Stun:
			StopAttack();
			CharacterState.CharacterActionState = ECharacterActionType::E_Stun;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			break;
		case ECharacterDebuffType::E_Slow:
			GetCharacterMovement()->MaxWalkSpeed *= Variable;
			CharacterStat.CharacterAtkSpeed *= Variable;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileSpeed;
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileSpeed *= Variable;
			}
			break;
		case ECharacterDebuffType::E_AttackDown:
			CharacterStat.CharacterAtkMultiplier *= Variable;
			break;
		case ECharacterDebuffType::E_DefenseDown:
			CharacterStat.CharacterDefense *= Variable;
			break;
		}
		Variable = ResetVal;
		TimerDelegate.BindUFunction(this, FName("ResetStatBuffState"), bIsDebuff, BuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
		return true;
	}

	/*---- 버프 타이머 세팅 ----------------------------*/
	else
	{
		Variable = 1.0f + FMath::Abs(Variable); //값 정제
		CharacterState.CharacterBuffState |= (1 << (int)BuffType);

		switch ((ECharacterBuffType)BuffType)
		{
			//----힐 버프-------------------
		case ECharacterBuffType::E_Healing:
			Variable -= 1.0f;
			TimerDelegate.BindUFunction(this, FName("TakeHeal"), Variable, true, BuffType);
			GetWorldTimerManager().SetTimer(BuffDebuffTimerHandleList[HEAL_BUFF_TIMER_IDX], TimerDelegate, 1.0f, true);
			break;
			//----스탯 조정 버프-------------------
		case ECharacterBuffType::E_AttackUp:
			CharacterStat.CharacterAtkMultiplier *= Variable;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.WeaponDamage *= Variable;
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileDamage *= Variable;
			}
			break;
		case ECharacterBuffType::E_DefenseUp:
			CharacterStat.CharacterDefense *= Variable;
			break;
		case ECharacterBuffType::E_SpeedUp:
			GetCharacterMovement()->MaxWalkSpeed *= Variable;
			CharacterStat.CharacterAtkSpeed *= Variable;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileSpeed *= Variable;
			}
			break;
		case ECharacterBuffType::E_Invincibility:
			CharacterStat.bIsInvincibility = true;
			break;
		case ECharacterBuffType::E_InfiniteAmmo:
			if (CharacterStat.bInfiniteAmmo) return false; //스탯으로써 무한 탄약 능력을 갖고 있다면?
			if (IsValid(GetWeaponActorRef()))
			{
				CharacterStat.bInfiniteAmmo = true;
				GetWeaponActorRef()->SetInfiniteAmmoMode(true);
			}
			break;
		}
		TimerDelegate.BindUFunction(this, FName("ResetStatBuffState"), bIsDebuff, BuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
		return true;
	}
	return false;
}

void ACharacterBase::ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal)
{
	if (bIsDebuff)
	{
		ResetVal = FMath::Max(0.0f, ResetVal);
		switch ((ECharacterDebuffType)BuffType)
		{
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			GetWorldTimerManager().ClearTimer(BuffDebuffTimerHandleList[DEBUFF_DAMAGE_TIMER_IDX]);
			break;
		case ECharacterDebuffType::E_Slow:
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
			CharacterStat.CharacterAtkSpeed /= ResetVal;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileSpeed /= ResetVal;
			}
			break;
		case ECharacterDebuffType::E_Stun:
			ResetActionState();
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;
		case ECharacterDebuffType::E_AttackDown:
			CharacterStat.CharacterAtkMultiplier /= ResetVal;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.WeaponDamage /= ResetVal;
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileDamage /= ResetVal;
			}
			break;
		case ECharacterDebuffType::E_DefenseDown:
			CharacterStat.CharacterDefense /= ResetVal;
			break;
		}
	}
	else
	{
		switch ((ECharacterBuffType)BuffType)
		{
		case ECharacterBuffType::E_Healing:
			GetWorldTimerManager().ClearTimer(BuffDebuffTimerHandleList[HEAL_BUFF_TIMER_IDX]);
			break;
		case ECharacterBuffType::E_DefenseUp:
			CharacterStat.CharacterDefense /= ResetVal;
			break;
		case ECharacterBuffType::E_AttackUp:
			CharacterStat.CharacterAtkMultiplier /= ResetVal;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.WeaponDamage /= ResetVal;
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileDamage /= ResetVal;
			}
			break;
		case ECharacterBuffType::E_SpeedUp:
			GetCharacterMovement()->MaxWalkSpeed /= ResetVal;
			CharacterStat.CharacterAtkSpeed /= ResetVal;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileSpeed /= ResetVal;
			}
			break;
		case ECharacterBuffType::E_Invincibility:
			CharacterStat.bIsInvincibility = false;
			ClearAllBuffTimer(true);
			break;
		case ECharacterBuffType::E_InfiniteAmmo:
			CharacterStat.bInfiniteAmmo = false;
			GetWeaponActorRef()->SetInfiniteAmmoMode(false);
			break;
		}
	}
	ClearBuffTimer(bIsDebuff, BuffType);
}

void ACharacterBase::ClearBuffTimer(bool bIsDebuff, uint8 BuffType)
{
	if (bIsDebuff)
	{
		CharacterState.CharacterDebuffState &= ~(1 << (int)BuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
	}
	else
	{
		CharacterState.CharacterBuffState &= ~(1 << (int)BuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
	} 
	GetWorldTimerManager().ClearTimer(GetBuffTimerHandleRef(bIsDebuff, BuffType));
}

void ACharacterBase::ClearAllBuffTimer(bool bIsDebuff)
{
	const uint16 startIdx = bIsDebuff ? MAX_BUFF_IDX + 2 : 0;
	const uint16 endIdx = bIsDebuff ? startIdx + MAX_DEBUFF_IDX : MAX_BUFF_IDX + 1;

	for (uint16 timerIdx = startIdx; timerIdx <= endIdx ; timerIdx++)
	{
		GetWorldTimerManager().ClearTimer(GetBuffTimerHandleRef(bIsDebuff, timerIdx));
	}
}

bool ACharacterBase::GetDebuffIsActive(ECharacterDebuffType DebuffType)
{
	if (CharacterState.CharacterDebuffState & (1 << (int)DebuffType)) return true;
	return false;
}

bool ACharacterBase::GetBuffIsActive(ECharacterBuffType BuffType)
{
	if (CharacterState.CharacterBuffState & (1 << (int)BuffType)) return true;
	return false;
}

float ACharacterBase::GetBuffRemainingTime(bool bIsDebuff, uint8 BuffType)
{
	FTimerHandle& targetBuffTimer = GetBuffTimerHandleRef(bIsDebuff, BuffType);
	return GetWorldTimerManager().GetTimerRemaining(targetBuffTimer);
}

FTimerHandle& ACharacterBase::GetBuffTimerHandleRef(bool bIsDebuff, uint8 BuffType)
{
	int16 targetListIdx = bIsDebuff ? BuffType + MAX_BUFF_IDX + 1 : BuffType;
	targetListIdx = BuffDebuffTimerHandleList.IsValidIndex(targetListIdx) ? targetListIdx : 0;
	return BuffDebuffTimerHandleList[targetListIdx];
}

void ACharacterBase::InitGamemodeRef()
{
	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ACharacterBase::ClearAllTimerHandle()
{
	ClearAllBuffTimer(false);
	ClearAllBuffTimer(true);
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


