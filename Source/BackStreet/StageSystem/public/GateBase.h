// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "Components/BoxComponent.h"
#include "GateBase.generated.h"

UCLASS()
class BACKSTREET_API AGateBase : public AActor
{
	GENERATED_BODY()


//------------ Global / Component -------------------
public:
	// Sets default values for this actor's properties
	AGateBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OverlapVolume;

//----------- �ٽ� ���� ---------------
public:
	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	//�� Ÿ���� �ε� / ���� Ÿ���� ��ε� �Ѵ�.
	UFUNCTION()
		void UpdateNewTile();

	// return ��ε��ų level instance
	UFUNCTION(BlueprintCallable)
		ULevelStreaming* UpdateGateInfo();

	UFUNCTION(BlueprintCallable)
		void CheckHaveToActive();

private:
	//��ε��ų ������ Ÿ��
	UPROPERTY()
		class ATileBase* CurrTileRef;


//----------- ���� ������ ���� ----------
public:
	//Ÿ�ϰ� �̵� �� ����� ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|VFX")
		class ULevelSequence* TileTravelEffectSequence;

	UFUNCTION()
		void InitTileTravelSequence();

private:
	UPROPERTY()
		class ULevelSequencePlayer* TravelSequencePlayer;
	
//------- Ÿ�̸� ���� ---------------------
private:
	UFUNCTION()
		void ClearAllTimerHandle();

	UPROPERTY()
		FTimerHandle TravelSequenceDelayHandle;

	UPROPERTY()
		FTimerHandle ResourceReturnTimerHandle;

//-------- �� ��-----------------------
private:
	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;
};
