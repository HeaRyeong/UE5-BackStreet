// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		virtual void BeginPlay() override;

public:
	//ĳ���Ͱ� �����ϰ� �ִ� Rotation�� ��ȯ
	UFUNCTION()
		FRotator GetAimingRotation();

	//UFUNCTION()
		//FRotator GetRightAnalogRotation();

	//ĳ���Ͱ� Ŀ���� ���� ������ �ٶ󺸴� Rotation�� ��ȯ
	UFUNCTION()
		FRotator GetRotationToCursor();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		bool bJoyStickMode = false;

private:
	UPROPERTY()
		class AMainCharacterBase* PlayerRef;
};
