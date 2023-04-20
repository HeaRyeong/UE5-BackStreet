// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "TimerManager.h"
#include "AbilityManagerBase.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInfoStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	//어빌리티의 ID
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0, UIMax = 10))
		uint8 AbilityId;

	//반복적인 연산이 필요한지? (도트 힐) , 현재 미사용
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		bool bIsRepetitive;

	//Callback 함수명 (TFunctionRef를 쓰는건 어떨지..) , 현재 미사용
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FName FuncName; 

	//어빌리티에 사용할 변수 (증가량), 현재 미사용
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		float Variable;

	//Repetitive 연산을 위한 TimerHandle
	UPROPERTY()
		FTimerHandle TimerHandle; 

		FTimerDelegate TimerDelegate;
};

UCLASS()
class BACKSTREET_API UAbilityManagerBase : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	UAbilityManagerBase();

public:
	//어빌리티 매니저 초기화, 부모 설정
	UFUNCTION()
		void InitAbilityManager(ACharacterBase* NewCharacter);

	//특정 어빌리티를 추가 (실패 시 false)
	UFUNCTION()
		bool TryAddNewAbility(const ECharacterAbilityType NewAbilityType);

	//소유하고 있는 어빌리티를 제거 (실패 시 false)
	UFUNCTION()
		bool TryRemoveAbility(const ECharacterAbilityType TargetAbilityType);

	//모든 어빌리티 초기화
	UFUNCTION()
		void ClearAllAbility();

protected
	//해당 Ability가 Activa한지 반환
	UFUNCTION()
		bool GetIsAbilityActive(const ECharacterAbilityType NewAbilityType);

	//배열로부터 AbilityInfo를 불러들임
	UFUNCTION()
		FAbilityInfoStruct GetAbilityInfo(const ECharacterAbilityType AbilityType);

protected:
	//최대 어빌리티 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 1, UIMax = 5))
		int32 MaxAbilityCount = 3;

private:
	//현재 플레이어가 소유한 어빌리티의 종류
	UPROPERTY()
		TArray<FAbilityInfoStruct> ActiveAbilityList;
	
	//소유자
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;
};
