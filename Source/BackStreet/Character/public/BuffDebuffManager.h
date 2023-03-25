// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "BuffDebuffManager.generated.h"
/**
 * 
 */
UCLASS()
class BACKSTREET_API ABuffDebuffManager : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ABuffDebuffManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//������ ������� �Ǵ�
	UFUNCTION()
		bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//���� or ����� ���¸� �ʱ�ȭ�Ѵ�
	UFUNCTION()
		void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal);

	//Ư�� Buff/Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	UFUNCTION()
		void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType);

public: 
	UFUNCTION()
		void InitBuffManager(class ACharacterBase* NewOwnerRef);

	UFUNCTION(BlueprintCallable)
		bool SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION(BlueprintCallable)
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//��� Buff/Debuff�� Ÿ�̸Ӹ� ����
	UFUNCTION()
		void ClearAllBuffDebuffTimer(bool bIsDebuff);

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

	UFUNCTION()
		int16 GetBuffDebuffInfoListIndex(bool bIsDebuff, uint8 BuffDebuffType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ABuffDebuffManager* GetCharacterBuffManager() { return this; }

private:
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		TArray<FTimerHandle> BuffDebuffTimerHandleList;

	UPROPERTY()
		TArray<float> BuffDebuffResetValueList;
};
