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

	UPROPERTY(EditDefaultsOnly)
		UChildActorComponent* BuffManagerComponent;

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
		void ResetActionState();

	//����� �������� ���� (��ȸ��)
	UFUNCTION()
		float TakeDebuffDamage(float DamageAmount, uint8 DebuffType, AActor* Causer);

	//�÷��̾ ü���� ȸ���� (��ȸ��)
	UFUNCTION()
		void TakeHeal(float HealAmount, bool bIsTimerEvent = false, uint8 BuffDebuffType = 0);

// ------- Character Stat/State ------------------------------
public:
	//ĳ������ ���� ������ �ʱ�ȭ
	UFUNCTION()
		void InitCharacterState();

	//ĳ������ ����/����� ������ ������Ʈ
	virtual	void AddNewBuffDebuff(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer = nullptr, float TotalTime = 0.0f, float Value = 0.0f);

	//������� Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//������ Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetBuffIsActive(ECharacterBuffType BuffType);

	//ĳ������ ������ ������Ʈ
	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FCharacterStatStruct NewStat);

	UFUNCTION(BlueprintCallable)
		void UpdateCharacterState(FCharacterStateStruct NewState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FCharacterStatStruct GetCharacterStat() { return CharacterStat; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FCharacterStateStruct GetCharacterState() { return CharacterState; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class ACharacterBuffManager* GetBuffManagerRef() { return BuffManagerRef; };

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

	UFUNCTION(BlueprintCallable)
		class AWeaponInventoryBase* GetInventoryRef();

	//���� Ref�� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		class AWeaponBase* GetWeaponActorRef();

	//����Action ������ Interval�� �����ϴ� Ÿ�̸Ӹ� ����
	UFUNCTION()
		void ResetAtkIntervalTimer();

// ----- ĳ���� �ִϸ��̼� -------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		TArray<class UAnimMontage*> AttackAnimMontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* ShootAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* RollAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* DieAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* ReloadAnimMontage;

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

	UPROPERTY()
		class ACharacterBuffManager* BuffManagerRef;

// ----- Ÿ�̸� ���� ---------------------------------
protected:
	virtual void ClearAllTimerHandle();

private:
	//���� �� ������ �ڵ�
	UPROPERTY()
		FTimerHandle AtkIntervalHandle;

	UPROPERTY()
		FTimerHandle ReloadTimerHandle;

	UPROPERTY()
		TArray<FTimerHandle> BuffDebuffTimerHandleList;

};