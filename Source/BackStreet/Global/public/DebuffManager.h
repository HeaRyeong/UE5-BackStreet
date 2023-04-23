// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "DebuffManager.generated.h"

USTRUCT(BlueprintType)
struct FDebuffTimerInfoStruct
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
class BACKSTREET_API UDebuffManager : public UObject
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	UDebuffManager();

	//������� �Ǵ�
	UFUNCTION()
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//����� ���¸� �ʱ�ȭ�Ѵ�
	UFUNCTION()
		void ResetStatDebuffState(ECharacterDebuffType DebuffType, class ACharacterBase* Target, float ResetVal);

	//Ư�� Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	UFUNCTION()
		void ClearDebuffTimer(ECharacterDebuffType DebuffType, class ACharacterBase* Target);

public: 
	UFUNCTION()
		void InitDebuffManager(class ABackStreetGameModeBase* NewGamemodeRef);

	//��� Debuff�� Ÿ�̸Ӹ� ����
	UFUNCTION()
		void ClearAllDebuffTimer();

	//������� Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION()
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType, class ACharacterBase* Target);

	//����� Ÿ�̸� �ڵ��� �����ڸ� ��ȯ
	UFUNCTION()
		FTimerHandle& GetDebuffTimerHandleRef(ECharacterDebuffType DebuffType, class ACharacterBase* Target);

	UFUNCTION()
		int16 GetDebuffInfoListIndex(ECharacterDebuffType DebuffType);

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
		TMap<int32, FDebuffTimerInfoStruct> TimerInfoMap;
};
