#include "../public/CharacterBase.h"
#include "../../Global/public/DebuffManager.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Item/public/WeaponInventoryBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Global/public/AssetManagerBase.h"
#include "Animation/AnimMontage.h"

// Sets default values
ACharacterBase::ACharacterBase()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Tags.Add("Character");

	InventoryComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("INVENTORY"));
	InventoryComponent->SetupAttachment(GetCapsuleComponent());

	BuffManagerComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("BUFF_MANAGER"));
	BuffManagerComponent->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	InitCharacterState();

	InventoryRef = Cast<AWeaponInventoryBase>(InventoryComponent->GetChildActor());
	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (IsValid(GetInventoryRef()))
	{
		GetInventoryRef()->SetOwner(this);
		GetInventoryRef()->InitInventory();
	}
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

bool ACharacterBase::TryAddNewDebuff(ECharacterDebuffType NewDebuffType, AActor* Causer, float TotalTime, float Value)
{
	if (!IsValid(GamemodeRef)) return false;
	if(!IsValid(GamemodeRef->GetGlobalDebuffManagerRef())) return false;

	GamemodeRef->GetGlobalDebuffManagerRef()->SetDebuffTimer(NewDebuffType, this, Causer, TotalTime, Value);
	return true;
}

bool ACharacterBase::GetDebuffIsActive(ECharacterDebuffType DebuffType)
{
	if (!IsValid(GamemodeRef)) return false;
	if (!IsValid(GamemodeRef->GetGlobalDebuffManagerRef())) return false;
	return	GamemodeRef->GetGlobalDebuffManagerRef()->GetDebuffIsActive(DebuffType, this);
}

void ACharacterBase::UpdateCharacterStat(FCharacterStatStruct NewStat)
{
	CharacterStat = NewStat;
	GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
}

void ACharacterBase::UpdateCharacterState(FCharacterStateStruct NewState)
{
	CharacterState = NewState;
}

void ACharacterBase::UpdateWeaponStat(FWeaponStatStruct NewStat)
{
	if (!IsValid(GetWeaponActorRef())) return;
	GetWeaponActorRef()->UpdateWeaponStat(NewStat);
}

void ACharacterBase::ResetActionState(bool bForceReset)
{
	if (CharacterState.CharacterActionState == ECharacterActionType::E_Die) return;
	if(!bForceReset && (CharacterState.CharacterActionState == ECharacterActionType::E_Stun
		|| CharacterState.CharacterActionState == ECharacterActionType::E_Reload)) return;

	CharacterState.CharacterActionState = ECharacterActionType::E_Idle;
	StopAttack();

	if (!GetWorldTimerManager().IsTimerActive(AtkIntervalHandle))
	{
		CharacterState.bCanAttack = true;
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator
								, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageAmount = DamageAmount - DamageAmount * CharacterStat.CharacterDefense;
	if (DamageAmount <= 0.0f || !IsValid(DamageCauser)) return 0.0f;
	if (CharacterStat.bIsInvincibility) return 0.0f;

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

float ACharacterBase::TakeDebuffDamage(float DamageAmount, ECharacterDebuffType DebuffType, AActor* Causer)
{
	if (!IsValid(Causer)) return 0.0f;
	TakeDamage(DamageAmount, FDamageEvent(), nullptr, Causer);
	return DamageAmount;
}

void ACharacterBase::TakeHeal(float HealAmountRate, bool bIsTimerEvent, uint8 BuffDebuffType)
{
	CharacterState.CharacterCurrHP += CharacterStat.CharacterMaxHP * HealAmountRate;
	CharacterState.CharacterCurrHP = FMath::Min(CharacterStat.CharacterMaxHP, CharacterState.CharacterCurrHP);
	return;
}

void ACharacterBase::Die()
{
	if (IsValid(GetInventoryRef()))
	{
		GetInventoryRef()->RemoveCurrentWeapon();
		GetInventoryRef()->Destroy();
	}
	CharacterState.CharacterActionState = ECharacterActionType::E_Die;
	CharacterStat.bIsInvincibility = true;
	ClearAllTimerHandle();
	GetCharacterMovement()->Deactivate();
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
	const float attackSpeed = FMath::Min(1.5f, CharacterStat.CharacterAtkSpeed * GetWeaponActorRef()->GetWeaponStat().WeaponAtkSpeedRate);

	if (GetWeaponActorRef()->GetWeaponStat().WeaponType == EWeaponType::E_Shoot)
	{
		PlayAnimMontage(ShootAnimMontage, attackSpeed + 0.75f);
	}
	else
	{	
		PlayAnimMontage(AttackAnimMontageArray[nextAnimIdx], attackSpeed + 0.75f);
	}
}

void ACharacterBase::Attack()
{
	if (!IsValid(GetWeaponActorRef())) return;
	
	const float attackSpeed = FMath::Min(1.5f, CharacterStat.CharacterAtkSpeed * GetWeaponActorRef()->GetWeaponStat().WeaponAtkSpeedRate);

	//GetWorldTimerManager().SetTimer(AtkIntervalHandle, this, &ACharacterBase::ResetAtkIntervalTimer
	//									, 1.0f, false, FMath::Max(0.0f, 1.5f - attackSpeed));
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

	float reloadTime = GetWeaponActorRef()->GetWeaponStat().LoadingDelayTime;
	if (IsValid(ReloadAnimMontage))
	{
		PlayAnimMontage(ReloadAnimMontage);
	}

	CharacterState.CharacterActionState = ECharacterActionType::E_Reload;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, FTimerDelegate::CreateLambda([&](){
		GetWeaponActorRef()->TryReload();
		CharacterState.CharacterActionState = ECharacterActionType::E_Idle;
	}), 1.0f, false, reloadTime);
}

void ACharacterBase::ResetAtkIntervalTimer()
{
	CharacterState.bCanAttack = true;
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
}

void ACharacterBase::InitDynamicMeshMaterial(UMaterialInterface* NewMaterial)
{
	if (NewMaterial == nullptr) return;

	for (int8 matIdx = 0; matIdx < GetMesh()->GetNumMaterials(); matIdx += 1)
	{
		CurrentDynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(matIdx, NewMaterial);
	}
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

void ACharacterBase::ClearAllTimerHandle()
{
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}
