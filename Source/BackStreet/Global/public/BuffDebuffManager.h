// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "BuffDebuffManager.generated.h"

USTRUCT(BlueprintType)
struct FBuffDebuffTimerInfoStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		TArray<FTimerHandle> TimerHandleList;

	UPROPERTY()
		TArray<float> ResetValueHandleList;
};

/** 
 */
UCLASS()
class BACKSTREET_API UBuffDebuffManager : public UObject
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	UBuffDebuffManager();

protected:
	//������ ������� �Ǵ�
	UFUNCTION()
		bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//���� or ����� ���¸� �ʱ�ȭ�Ѵ�
	UFUNCTION()
		void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target, float ResetVal);

	//Ư�� Buff/Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	UFUNCTION()
		void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target);

public: 
	UFUNCTION()
		void InitBuffManager(class ABackStreetGameModeBase* NewGamemodeRef);

	UFUNCTION(BlueprintCallable)
		bool SetBuffTimer(ECharacterBuffType BuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION(BlueprintCallable)
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//��� Buff/Debuff�� Ÿ�̸Ӹ� ����
	UFUNCTION()
		void ClearAllBuffDebuffTimer(bool bIsDebuff);

	//������� Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION()
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType, class ACharacterBase* Target);

	//������ Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION()
		bool GetBuffIsActive(ECharacterBuffType BuffType, class ACharacterBase* Target);

	//����/����� ���� �ð��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target);

	//���� / ����� Ÿ�̸� �ڵ��� �����ڸ� ��ȯ
	UFUNCTION()
		FTimerHandle& GetBuffDebuffTimerHandleRef(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target);

	UFUNCTION()
		int16 GetBuffDebuffInfoListIndex(bool bIsDebuff, uint8 BuffDebuffType);

private:
	UFUNCTION()
		TArray<FTimerHandle>& GetTimerHandleListRef(class ACharacterBase* Target);

	UFUNCTION()
		TArray<float>& GetResetValueListRef(class ACharacterBase* Target);

	UFUNCTION()
		void AddNewTimerList(class ACharacterBase* Target);

private:
	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		TMap<int32, FBuffDebuffTimerInfoStruct> TimerInfoMap;
};
