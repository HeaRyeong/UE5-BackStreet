// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/CharacterBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Animation/AnimMontage.h"


/*----  UPROPERTY 내에서는 다차원 배열을 지원하지 않음 ----*/
FTimerHandle BuffTimerHandle[2][10];
float BuffRemainingTime[2][10];
/*----------------------------------------------------*/


// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("WEAPON"));
	WeaponActor->SetupAttachment(GetMesh(), FName("Weapon_R"));

	
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
	if (!IsValid(Causer) || BuffRemainingTime[1][(int)DebuffType] <= 0.0f) return 0.0f;

	BuffRemainingTime[1][(int)DebuffType] -= 1.0f;
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
	CharacterState.CharacterCurrHP = FMath::Min(CharacterStat.CharacterMaxHP
												, CharacterState.CharacterCurrHP);
	if (bIsTimerEvent)
	{
		BuffRemainingTime[0][(int)BuffType] -= 1.0f;
		if (BuffRemainingTime[0][(int)BuffType] <= 0.0f)
		{
			ClearBuffTimer(false, BuffType);
		}
		return;
	}
}

void ACharacterBase::Die()
{
	CharacterState.CharacterActionState = ECharacterActionType::E_Die;
	CharacterStat.bIsInvincibility = true;
	ClearAllBuffTimer(true);
	ClearAllBuffTimer(false);
	ClearAllTimerHandle();
	
	GetCharacterMovement()->Deactivate();
	bUseControllerRotationYaw = false;
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

void ACharacterBase::SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime, float Variable)
{
	FTimerDelegate TimerDelegate;
	FTimerHandle& timerHandle = BuffTimerHandle[bIsDebuff][(int)BuffType];

	if ((bIsDebuff && GetDebuffIsActive((ECharacterDebuffType)BuffType))
		&& (!bIsDebuff && GetBuffIsActive((ECharacterBuffType)BuffType)))
	{
		BuffRemainingTime[bIsDebuff][(int)BuffType] += TotalTime;
		return;
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
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 1.0f, true);
			return;
		//----스탯 조정 디버프-------------------
		case ECharacterDebuffType::E_Stun:
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
		TimerDelegate.BindUFunction(this, FName("ResetStatBuffState"), bIsDebuff, BuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
		return;
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
			TimerDelegate.BindUFunction(this, FName("TakeHeal"), Variable, true, BuffType);
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 1.0f, true);
			return;

		//----스탯 조정 버프-------------------
		case ECharacterBuffType::E_AttackUp:
			CharacterStat.CharacterAtkMultiplier *= Variable;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.WeaponDamage *= Variable;
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
			if (CharacterStat.bInfiniteAmmo) return; //스탯으로써 무한 탄약 능력을 갖고 있다면?
			if (IsValid(GetWeaponActorRef()))
			{
				CharacterStat.bInfiniteAmmo = true;
				GetWeaponActorRef()->SetInfiniteAmmoMode(true);
			}
			break;
		}
		TimerDelegate.BindUFunction(this, FName("ResetStatBuffState"), bIsDebuff, BuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
	}
}

void ACharacterBase::ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal)
{
	if (bIsDebuff)
	{
		ResetVal = FMath::Max(0.0f, ResetVal);
		switch ((ECharacterDebuffType)BuffType)
		{
		case ECharacterDebuffType::E_Slow:
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
			CharacterStat.CharacterAtkSpeed /= ResetVal;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.ProjectileStat.ProjectileSpeed /= ResetVal;
			}
			break;
		case ECharacterDebuffType::E_Stun:
			//WakeUp Animation
			CharacterState.CharacterActionState = ECharacterActionType::E_Idle;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;
		case ECharacterDebuffType::E_AttackDown:
			CharacterStat.CharacterAtkMultiplier /= ResetVal;
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
		case ECharacterBuffType::E_DefenseUp:
			CharacterStat.CharacterDefense /= ResetVal;
			break;
		case ECharacterBuffType::E_AttackUp:
			CharacterStat.CharacterAtkMultiplier /= ResetVal;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.WeaponDamage /= ResetVal;
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
	BuffRemainingTime[bIsDebuff][(int)BuffType] = 0.0f;
	GetWorldTimerManager().ClearTimer(BuffTimerHandle[bIsDebuff][(int)BuffType]);
}

void ACharacterBase::ClearAllBuffTimer(bool bIsDebuff)
{
	for (int timerIdx = 0; timerIdx < 10; timerIdx++)
	{
		GetWorldTimerManager().ClearTimer(BuffTimerHandle[bIsDebuff][timerIdx]);
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


