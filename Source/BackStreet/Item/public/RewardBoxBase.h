#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RewardBoxBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateInteraction, class AMainCharacterBase*, PlayerCharacterRef);

UCLASS()
class BACKSTREET_API ARewardBoxBase : public AActor
{
	GENERATED_BODY()

/*-------- Delegate ----------------------- */
public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateInteraction OnPlayerBeginInteract;

/*-------- ����, ������Ʈ ----------------------- */
public:
	// Sets default values for this actor's properties
	ARewardBoxBase();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* OverlapVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UWidgetComponent* InfoWidgetComponent;

/*------- �ֿ� ��� ---------------------------*/
protected:
	//���� ������Ʈ �ʱ�ȭ
	UFUNCTION(BlueprintImplementableEvent)
		void InitializeWidgetComponent();

	//���� ������Ʈ Ȱ��ȭ(��Ȱ��ȭ)
	UFUNCTION(BlueprintImplementableEvent)
		void ActivateWidgetComponent(bool bDeactivateFlag);

	//�÷��̾� ������ ��, ������ ���
	UFUNCTION()
		void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
								, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�÷��̾� ������ ���� ��, ������ ����
	UFUNCTION()
		void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
								, int32 OtherBodyIndex);

	//�÷��̾�� �����Ƽ�� �ִ� ���� �õ��Ѵ�.
	UFUNCTION()
		void AddAbilityToPlayer(class AMainCharacterBase* PlayerCharacterRef);

	//�������� �����Ƽ Ÿ���� ����´�.
	UFUNCTION(BlueprintCallable)
		void SelectRandomAbilityIdx();

// ----- Asset -----------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		class UParticleSystem* DestroyEffectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* DestroyEffectSound;

/*------- ������Ƽ ---------------------------*/
protected:
	//������ ���� Ÿ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterAbilityType AbilityType; 

private:
	//������ ��� ������ true�� �ٲ�� ��ȣ�ۿ�(�����Ƽ �߰�)�� ����
	UPROPERTY()
		bool bIsReadyToInteract = false;
};
