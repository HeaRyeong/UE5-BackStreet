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

	//버프와 디버프를 건다
	UFUNCTION()
		bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//버프 or 디버프 상태를 초기화한다
	UFUNCTION()
		void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal);

	//특정 Buff/Debuff의 타이머를 해제한다.
	UFUNCTION()
		void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType);

public: 
	UFUNCTION()
		void InitBuffManager(class ACharacterBase* NewOwnerRef);

	UFUNCTION(BlueprintCallable)
		bool SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION(BlueprintCallable)
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//모든 Buff/Debuff의 타이머를 해제
	UFUNCTION()
		void ClearAllBuffDebuffTimer(bool bIsDebuff);

	//디버프가 활성화 되어있는지 반환
	UFUNCTION()
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//버프가 활성화 되어있는지 반환
	UFUNCTION()
		bool GetBuffIsActive(ECharacterBuffType BuffType);

	//버프/디버프 남은 시간을 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType);

	//버프 / 디버프 타이머 핸들의 참조자를 반환
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
