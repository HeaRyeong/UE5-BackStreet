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

	//디버프를 건다
	UFUNCTION()
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//디버프 상태를 초기화한다
	UFUNCTION()
		void ResetStatDebuffState(ECharacterDebuffType DebuffType, class ACharacterBase* Target, float ResetVal);

	//특정 Debuff의 타이머를 해제한다.
	UFUNCTION()
		void ClearDebuffTimer(ECharacterDebuffType DebuffType, class ACharacterBase* Target);

public: 
	UFUNCTION()
		void InitDebuffManager(class ABackStreetGameModeBase* NewGamemodeRef);

	//모든 Debuff의 타이머를 해제
	UFUNCTION()
		void ClearAllDebuffTimer();

	//디버프가 활성화 되어있는지 반환
	UFUNCTION()
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType, class ACharacterBase* Target);

	//디버프 타이머 핸들의 참조자를 반환
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
