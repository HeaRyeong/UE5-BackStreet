// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageManagerBase.generated.h"
#define GridSize 3;

UCLASS()
class BACKSTREET_API AStageManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void InitStageManager(class AGridBase* Chapter);

	UFUNCTION()
		void CleanManager();

	UFUNCTION(BlueprintCallable)
		void MoveStage(uint8 Dir);

	UFUNCTION(BlueprintCallable)
		TArray<ATileBase*> GetStages() { return Stages; }

	UFUNCTION(BlueprintCallable)
		ATileBase* GetCurrentStage() { return CurrentTile; }

private:
	UFUNCTION()
		void SetMissionStages();

	UFUNCTION()
		void LoadStage(class ATileBase* targetStage);

	UFUNCTION()
		void UnLoadStage(class ATileBase* targetStage);
	
	UFUNCTION(BlueprintCallable)
		class ATileBase* GetStage(int32 XPosition, int32 YPosition);

private:
	UPROPERTY()
		TArray<class ATileBase*> Stages;

	UPROPERTY(VisibleAnywhere)
		class ATileBase* CurrentTile;

	// Ref
private:
	UPROPERTY()
		class ACharacterBase* CharacterRef;

	UPROPERTY()
		class ALevelScriptInGame* InGameScriptRef;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Stages")
		TArray<FName> Maps;


};
