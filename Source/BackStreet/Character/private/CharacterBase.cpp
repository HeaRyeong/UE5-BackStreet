#include "../public/CharacterBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Item/public/WeaponInventoryBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Global/public/AssetManagerBase.h"
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
	this->Tags.Add("Character");

	InventoryComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("INVENTORY"));
	InventoryComponent->SetupAttachment(GetCapsuleComponent());

	for (int newTimerIdx = 0; newTimerIdx < 18; newTimerIdx += 1)
	{
		BuffDebuffTimerHandleList.Add(FTimerHandle());
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitCharacterState();

	InventoryRef = Cast<AWeaponInventoryBase>(InventoryComponent->GetChildActor());
	if (IsValid(GetInventoryRef()))
	{
		GetInventoryRef()->SetOwner(this);
		GetInventoryRef()->InitInventory();
	}
	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
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
	if (CharacterState.CharacterActionState == ECharacterActionType::E_Stun
		|| CharacterState.CharacterActionState == ECharacterActionType::E_Die) return;

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
	if (!IsValid(Causer)) return 0.0f;
	TakeDamage(DamageAmount, FDamageEvent(), nullptr, Causer);
	return DamageAmount;
}

void ACharacterBase::TakeHeal(float HealAmount, bool bIsTimerEvent, uint8 BuffDebuffType)
{
	CharacterState.CharacterCurrHP += HealAmount;
	CharacterState.CharacterCurrHP = FMath::Min(CharacterStat.CharacterMaxHP, CharacterState.CharacterCurrHP);
	return;
}

void ACharacterBase::Die()
{
	CharacterState.CharacterActionState = ECharacterActionType::E_Die;
	CharacterStat.bIsInvincibility = true;
	ClearAllBuffDebuffTimer(true);
	ClearAllBuffDebuffTimer(false);
	ClearAllTimerHandle();
	
	GetCharacterMovement()->Deactivate();
	if (IsValid(GetInventoryRef()))
	{
		GetInventoryRef()->RemoveCurrentWeapon();
		GetInventoryRef()->Destroy();
	}
	bUseControllerRotationYaw = false;

	if (DieAnimMontage != nullptr)
	{
		PlayAnimMontage(DieAnimMontage);
	}
	else
	{
		Destroy();
	}
}

void ACharacterBase::TryAttack()
{
	if (AttackAnimMontageArray.Num() <= 0) return;
	if (!IsValid(GetWeaponActorRef())) return;
	if (GetWorldTimerManager().IsTimerActive(AtkIntervalHandle)) return;
	if (!CharacterState.bCanAttack || !GetIsActionActive(ECharacterActionType::E_Idle)) return;
	
	CharacterState.bCanAttack = false; //공격간 Delay,Interval 조절을 위해 세팅
	CharacterState.CharacterActionState = ECharacterActionType::E_Attack;

	const int32 nextAnimIdx = GetWeaponActorRef()->GetCurrentComboCnt() % AttackAnimMontageArray.Num();
	PlayAnimMontage(AttackAnimMontageArray[nextAnimIdx]);
}

void ACharacterBase::Attack()
{
	if (!IsValid(GetWeaponActorRef())) return;
	GetWorldTimerManager().SetTimer(AtkIntervalHandle, this, &ACharacterBase::ResetAtkIntervalTimer
										, 1.0f, false, FMath::Max(0.0f, 1.25f - CharacterStat.CharacterAtkSpeed));
	GetWeaponActorRef()->Attack();
}
 
void ACharacterBase::StopAttack()
{
	if (!IsValid(GetWeaponActorRef())) return;
	GetWeaponActorRef()->StopAttack();
}

void ACharacterBase::TryReload()
{
	if (!IsValid(GetWeaponActorRef())) return;
	if (!GetWeaponActorRef()->GetCanReload())
	{
		UE_LOG(LogTemp, Warning, TEXT("CAN'T RELOAD"));
		return;
	}

	float reloadTime = GetWeaponActorRef()->WeaponStat.LoadingDelayTime;
	if (IsValid(ReloadAnimMontage))
	{
		PlayAnimMontage(ReloadAnimMontage);
	}

	CharacterState.CharacterActionState = ECharacterActionType::E_Reload;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, FTimerDelegate::CreateLambda([&](){
		GetWeaponActorRef()->TryReload();
		ResetActionState();
	}), 1.0f, false, reloadTime);
}

void ACharacterBase::ResetAtkIntervalTimer()
{
	CharacterState.bCanAttack = true;
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
}

bool ACharacterBase::EquipWeapon(AWeaponBase* TargetWeapon)
{
	TargetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Weapon_R");
	TargetWeapon->SetActorRelativeLocation(FVector(0.0f), false);
	TargetWeapon->SetOwnerCharacter(this);
	return true;
}

bool ACharacterBase::PickWeapon(int32 NewWeaponID)
{
	if (!IsValid(GetInventoryRef())) return false;
	bool result = InventoryRef->AddWeapon(NewWeaponID);
	return result;
}

void ACharacterBase::SwitchToNextWeapon()
{
	if (!IsValid(GetInventoryRef())) return;
	if (!IsValid(GetWeaponActorRef())) return;
	InventoryRef->SwitchToNextWeapon();
}

void ACharacterBase::DropWeapon()
{
	if (!IsValid(GetInventoryRef())) return;

	GetInventoryRef()->RemoveCurrentWeapon();
	/*---- 현재 무기를 월드에 버리는 기능은 미구현 -----*/
}

AWeaponInventoryBase* ACharacterBase::GetInventoryRef()
{
	if (!IsValid(InventoryRef)) return nullptr;
	return InventoryRef;
}

AWeaponBase* ACharacterBase::GetWeaponActorRef()
{
	if (!IsValid(GetInventoryRef())) return nullptr;

	return GetInventoryRef()->GetCurrentWeaponRef();
}

bool ACharacterBase::SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Variable)
{
	FTimerDelegate TimerDelegate;
	FTimerHandle& timerHandle = GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType);

	if ((bIsDebuff && GetDebuffIsActive((ECharacterDebuffType)BuffDebuffType))
		|| (!bIsDebuff && GetBuffIsActive((ECharacterBuffType)BuffDebuffType)))
	{
		const float newTime = GetWorldTimerManager().GetTimerRemaining(timerHandle) + TotalTime;
		GetWorldTimerManager().SetTimer(timerHandle, 1.0f, false, newTime);
		return true;
	}

	/*---- 디버프 타이머 세팅 ----------------------------*/
	if (bIsDebuff)
	{
		Variable = FMath::Min(1.0f, FMath::Abs(Variable)); //값 정제
		CharacterState.CharacterDebuffState |= (1 << (int)BuffDebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)

		switch ((ECharacterDebuffType)BuffDebuffType)
		{
			//----데미지 디버프-------------------
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			//SpawnBuffParticle(E_Flame, TotalTime) //TimerDelegate에 걸어도 됨
			TimerDelegate.BindUFunction(this, FName("TakeDebuffDamage"), Variable, BuffDebuffType, Causer);
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
				GetWeaponActorRef()->WeaponStat.WeaponAtkSpeedRate *= Variable;
			}
			break;
		case ECharacterDebuffType::E_AttackDown:
			CharacterStat.CharacterAtkMultiplier *= Variable;
			break;
		case ECharacterDebuffType::E_DefenseDown:
			CharacterStat.CharacterDefense *= Variable;
			break;
		}
		TimerDelegate.BindUFunction(this, FName("ResetStatBuffDebuffState"), bIsDebuff, BuffDebuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
		return true;
	}

	/*---- 버프 타이머 세팅 ----------------------------*/
	else
	{
		Variable = 1.0f + FMath::Abs(Variable); //값 정제
		CharacterState.CharacterBuffState |= (1 << (int)BuffDebuffType);

		switch ((ECharacterBuffType)BuffDebuffType)
		{
			//----힐 버프-------------------
		case ECharacterBuffType::E_Healing:
			Variable -= 1.0f;
			TimerDelegate.BindUFunction(this, FName("TakeHeal"), Variable, true, BuffDebuffType);
			GetWorldTimerManager().SetTimer(BuffDebuffTimerHandleList[HEAL_BUFF_TIMER_IDX], TimerDelegate, 1.0f, true);
			break;
			//----스탯 조정 버프-------------------
		case ECharacterBuffType::E_AttackUp:
			CharacterStat.CharacterAtkMultiplier *= Variable;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.WeaponDamageRate *= Variable;
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
				GetWeaponActorRef()->WeaponStat.WeaponAtkSpeedRate *= Variable;
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
		TimerDelegate.BindUFunction(this, FName("ResetStatBuffDebuffState"), bIsDebuff, BuffDebuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
		return true;
	}
	return false;
}

void ACharacterBase::ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal)
{
	if (bIsDebuff)
	{
		ResetVal = FMath::Max(0.0f, ResetVal);
		switch ((ECharacterDebuffType)BuffDebuffType)
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
				GetWeaponActorRef()->WeaponStat.WeaponAtkSpeedRate /= ResetVal;
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
				GetWeaponActorRef()->WeaponStat.WeaponDamageRate /= ResetVal;
			}
			break;
		case ECharacterDebuffType::E_DefenseDown:
			CharacterStat.CharacterDefense /= ResetVal;
			break;
		}
	}
	else
	{
		switch ((ECharacterBuffType)BuffDebuffType)
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
				GetWeaponActorRef()->WeaponStat.WeaponDamageRate /= ResetVal;
			}
			break;
		case ECharacterBuffType::E_SpeedUp:
			GetCharacterMovement()->MaxWalkSpeed /= ResetVal;
			CharacterStat.CharacterAtkSpeed /= ResetVal;
			if (IsValid(GetWeaponActorRef()))
			{
				GetWeaponActorRef()->WeaponStat.WeaponAtkSpeedRate /= ResetVal;
			}
			break;
		case ECharacterBuffType::E_Invincibility:
			CharacterStat.bIsInvincibility = false;
			ClearAllBuffDebuffTimer(true);
			break;
		case ECharacterBuffType::E_InfiniteAmmo:
			CharacterStat.bInfiniteAmmo = false;
			GetWeaponActorRef()->SetInfiniteAmmoMode(false);
			break;
		}
	}
	ClearBuffDebuffTimer(bIsDebuff, BuffDebuffType);
}

void ACharacterBase::ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType)
{
	if (bIsDebuff)
	{
		CharacterState.CharacterDebuffState &= ~(1 << (int)BuffDebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
	}
	else
	{
		CharacterState.CharacterBuffState &= ~(1 << (int)BuffDebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
	} 
	GetWorldTimerManager().ClearTimer(GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType));
}

void ACharacterBase::ClearAllBuffDebuffTimer(bool bIsDebuff)
{
	const uint16 startIdx = bIsDebuff ? MAX_BUFF_IDX + 2 : 0;
	const uint16 endIdx = bIsDebuff ? startIdx + MAX_DEBUFF_IDX : MAX_BUFF_IDX + 1;

	for (uint16 timerIdx = startIdx; timerIdx <= endIdx ; timerIdx++)
	{
		GetWorldTimerManager().ClearTimer(GetBuffDebuffTimerHandleRef(bIsDebuff, timerIdx));
	}
}

bool ACharacterBase::SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	return SetBuffDebuffTimer(false, (uint8)BuffType, Causer, TotalTime, Variable);
}

bool ACharacterBase::SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	return SetBuffDebuffTimer(true, (uint8)DebuffType, Causer, TotalTime, Variable);
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

float ACharacterBase::GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType)
{
	FTimerHandle& targetBuffTimer = GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType);
	return GetWorldTimerManager().GetTimerRemaining(targetBuffTimer);
}

FTimerHandle& ACharacterBase::GetBuffDebuffTimerHandleRef(bool bIsDebuff, uint8 BuffDebuffType)
{
	int16 targetListIdx = bIsDebuff ? BuffDebuffType + MAX_BUFF_IDX + 1 : BuffDebuffType;
	targetListIdx = BuffDebuffTimerHandleList.IsValidIndex(targetListIdx) ? targetListIdx : 0;
	return BuffDebuffTimerHandleList[targetListIdx];
}

void ACharacterBase::ClearAllTimerHandle()
{
	ClearAllBuffDebuffTimer(false);
	ClearAllBuffDebuffTimer(true);
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}
