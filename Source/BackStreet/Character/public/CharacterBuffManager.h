// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "CharacterBuffManager.generated.h"
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateBuffEnd);

UCLASS()
class BACKSTREET_API ACharacterBuffManager : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ACharacterBuffManager();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateBuffEnd BuffEmitterDeactivateDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: 
	//������ ������� �Ǵ�
	UFUNCTION()
		bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//���� or ����� ���¸� �ʱ�ȭ�Ѵ�
	UFUNCTION()
		void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal);

	//Ư�� Buff/Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	UFUNCTION()
		void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType);
	
	//��� Buff/Debuff�� Ÿ�̸Ӹ� ����
	UFUNCTION()
		void ClearAllBuffDebuffTimer(bool bIsDebuff);

	UFUNCTION(BlueprintCallable)
		bool SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION(BlueprintCallable)
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//������� Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION()
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//������ Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION()
		bool GetBuffIsActive(ECharacterBuffType BuffType);

	//����/����� ���� �ð��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType);

	//���� / ����� Ÿ�̸� �ڵ��� �����ڸ� ��ȯ
	UFUNCTION()
		FTimerHandle& GetBuffDebuffTimerHandleRef(bool bIsDebuff, uint8 BuffDebuffType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ACharacterBuffManager* GetCharacterBuffManager() { return this; }

private:
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		TArray<FTimerHandle> BuffDebuffTimerHandleList;
};
