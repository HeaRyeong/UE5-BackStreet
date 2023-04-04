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
	//버프와 디버프를 건다
	UFUNCTION()
		bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//버프 or 디버프 상태를 초기화한다
	UFUNCTION()
		void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target, float ResetVal);

	//특정 Buff/Debuff의 타이머를 해제한다.
	UFUNCTION()
		void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target);

public: 
	UFUNCTION()
		void InitBuffManager(class ABackStreetGameModeBase* NewGamemodeRef);

	UFUNCTION(BlueprintCallable)
		bool SetBuffTimer(ECharacterBuffType BuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION(BlueprintCallable)
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, class ACharacterBase* Target, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//모든 Buff/Debuff의 타이머를 해제
	UFUNCTION()
		void ClearAllBuffDebuffTimer(bool bIsDebuff);

	//디버프가 활성화 되어있는지 반환
	UFUNCTION()
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType, class ACharacterBase* Target);

	//버프가 활성화 되어있는지 반환
	UFUNCTION()
		bool GetBuffIsActive(ECharacterBuffType BuffType, class ACharacterBase* Target);

	//버프/디버프 남은 시간을 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType, class ACharacterBase* Target);

	//버프 / 디버프 타이머 핸들의 참조자를 반환
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
