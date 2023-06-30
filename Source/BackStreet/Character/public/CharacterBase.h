#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class BACKSTREET_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

//----- Global / Component ----------
public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
		UChildActorComponent* InventoryComponent;	

// ------- Character Action �⺻ ------- 
public:
	//Input�� Binding �Ǿ� ������ �õ� (AnimMontage�� ȣ��)
	virtual void TryAttack();

	//AnimNotify�� Binding �Ǿ� ���� ������ ����
	virtual void Attack();

	virtual void StopAttack();

	virtual void TryReload();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
		, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die();

	//�÷��̾ ���� �ش� Action�� �����ϰ� �ִ��� ��ȯ
	UFUNCTION(BlueprintCallable)
		bool GetIsActionActive(ECharacterActionType Type) { return CharacterState.CharacterActionState == Type; }

	//�÷��̾��� ActionState�� Idle�� ��ȯ�Ѵ�.
	UFUNCTION(BlueprintCallable)
		void ResetActionState(bool bForceReset = false);

	//����� �������� ���� (��ȸ��)
	UFUNCTION(BlueprintCallable)
		float TakeDebuffDamage(float DamageAmount, ECharacterDebuffType DebuffType, AActor* Causer);

	//�÷��̾ ü���� ȸ���� (��ȸ��)
	UFUNCTION()
		void TakeHeal(float HealAmountRate, bool bIsTimerEvent = false, uint8 BuffDebuffType = 0);

// ------- Character Stat/State ------------------------------
public:
	//ĳ������ ���� ������ �ʱ�ȭ
	UFUNCTION()
		void InitCharacterState();

	//ĳ������ ����� ������ ������Ʈ
	UFUNCTION(BlueprintCallable)
		virtual	bool TryAddNewDebuff(ECharacterDebuffType NewDebuffType, AActor* Causer = nullptr, float TotalTime = 0.0f, float Value = 0.0f);

	//������� Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//ĳ������ ������ ������Ʈ
	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FCharacterStatStruct NewStat);

	UFUNCTION(BlueprintCallable)
		void UpdateCharacterState(FCharacterStateStruct NewState);

	UFUNCTION(BlueprintCallable)
		void UpdateWeaponStat(FWeaponStatStruct NewStat);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FCharacterStatStruct GetCharacterStat() { return CharacterStat; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FCharacterStateStruct GetCharacterState() { return CharacterState; }

// ------ ���� ���� -------------------------------------------
public:
	UFUNCTION()
		bool EquipWeapon(class AWeaponBase* TargetWeapon);

	//���⸦ ���´�. �κ��丮�� �� á�ٸ� false�� ��ȯ
	UFUNCTION(BlueprintCallable)
		bool PickWeapon(int32 NewWeaponID);

	//���� ����� ��ȯ�Ѵ�. ��ȯ�� �����ϸ� false�� ��ȯ
	virtual void SwitchToNextWeapon();

	//���⸦ Drop�Ѵ�. (���忡�� �ƿ� �������.)
	virtual void DropWeapon();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class AWeaponInventoryBase* GetInventoryRef();

	//���� Ref�� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		class AWeaponBase* GetWeaponActorRef();

	//����Action ������ Interval�� �����ϴ� Ÿ�̸Ӹ� ����
	UFUNCTION()
		void ResetAtkIntervalTimer();

// ----- �ִϸ��̼� ���� -------------------
protected:
	//�ִϸ��̼�, VFX, ����ť �� ����
	UPROPERTY()
		struct FCharacterAnimAssetInfoStruct AnimAssetData;

// ----- VFX ------------------- -> �� �ִϸ��̼� ó�� ���� ����
protected:
	UFUNCTION()
		void InitDynamicMeshMaterial(UMaterialInterface* NewMaterial);

	UPROPERTY()
		class UMaterialInstanceDynamic* CurrentDynamicMaterial;

// ------ �� �� ĳ���� ������Ƽ  ---------------
protected:
	//ĳ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStatStruct CharacterStat;

	//ĳ������ ���� ����
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
		FCharacterStateStruct CharacterState;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		class AWeaponInventoryBase* InventoryRef;

// ----- Ÿ�̸� ���� ---------------------------------
protected:
	virtual void ClearAllTimerHandle();

	//���� �� ������ �ڵ�
	UPROPERTY()
		FTimerHandle AtkIntervalHandle;

	UPROPERTY()
		FTimerHandle ReloadTimerHandle;
};