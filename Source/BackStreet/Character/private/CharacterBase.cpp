#include "../public/CharacterBase.h"
#include "../public/CharacterBuffManager.h"
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
	BuffManagerRef = Cast<ACharacterBuffManager>(BuffManagerComponent->GetChildActor());
	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (IsValid(GetInventoryRef()) && IsValid(BuffManagerRef))
	{
		GetInventoryRef()->SetOwner(this);
		GetInventoryRef()->InitInventory();
		BuffManagerRef->InitBuffManager(this);
	}
	//GamemodeRef->ClearResourceDelegate.AddDynamic(this, &ACharacterBase::ClearAllTimerHandle);
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

bool ACharacterBase::AddNewBuffDebuff(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Value)
{
	if (!IsValid(GetBuffManagerRef())) return false;

	bool result = !bIsDebuff ? GetBuffManagerRef()->SetBuffTimer((ECharacterBuffType)BuffDebuffType, Causer, TotalTime, Value)
							: GetBuffManagerRef()->SetDebuffTimer((ECharacterDebuffType)BuffDebuffType, Causer, TotalTime, Value);
	return result;
}

bool ACharacterBase::GetDebuffIsActive(ECharacterDebuffType DebuffType)
{
	if (!IsValid(GetBuffManagerRef())) return false;
	return GetBuffManagerRef()->GetDebuffIsActive(DebuffType);
}

bool ACharacterBase::GetBuffIsActive(ECharacterBuffType BuffType)
{
	if (!IsValid(GetBuffManagerRef())) return false;
	return GetBuffManagerRef()->GetBuffIsActive(BuffType);
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

void ACharacterBase::ResetActionState()
{
	if (CharacterState.CharacterActionState == ECharacterActionType::E_Stun
		|| CharacterState.CharacterActionState == ECharacterActionType::E_Die
		|| CharacterState.CharacterActionState == ECharacterActionType::E_Reload) return;

	StopAttack();
	CharacterState.CharacterActionState = ECharacterActionType::E_Idle;
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
	if (IsValid(GetBuffManagerRef()))
	{
		GetBuffManagerRef()->ClearAllBuffDebuffTimer(true);
		GetBuffManagerRef()->ClearAllBuffDebuffTimer(false);
	}
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

	if (GetWeaponActorRef()->GetWeaponStat().WeaponType == EWeaponType::E_Shoot)
	{
		PlayAnimMontage(ShootAnimMontage, CharacterStat.CharacterAtkSpeed + 1.0f);
	}
	else
	{
		PlayAnimMontage(AttackAnimMontageArray[nextAnimIdx], CharacterStat.CharacterAtkSpeed + 1.0f);
	}
}

void ACharacterBase::Attack()
{
	if (!IsValid(GetWeaponActorRef())) return;
	GetWorldTimerManager().SetTimer(AtkIntervalHandle, this, &ACharacterBase::ResetAtkIntervalTimer
										, 1.0f, false, FMath::Max(0.0f, 1.0f - CharacterStat.CharacterAtkSpeed));
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
	GetBuffManagerRef()->ClearAllBuffDebuffTimer(false);
	GetBuffManagerRef()->ClearAllBuffDebuffTimer(true);
	GetWorldTimerManager().ClearTimer(AtkIntervalHandle);
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}
