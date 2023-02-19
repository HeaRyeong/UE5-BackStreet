// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "UObject/WeakObjectPtr.h"
#include "StageManagerBase.generated.h"
#define GridSize 3


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

	UFUNCTION()
		void UnLoadStage();

	UFUNCTION(BlueprintCallable)
		TArray<ATileBase*> GetStages() { return Stages; }

	UFUNCTION(BlueprintCallable)
		ATileBase* GetCurrentStage() { return CurrentTile; }

	UFUNCTION(BlueprintCallable)
		ATileBase* GetUnloadStage() { return UnloadTile; }

	UFUNCTION(BlueprintCallable)
		EDirection GetMoveDir() { return MoveDir; }

private:
	UFUNCTION()
		void SetMissionStages();

	UFUNCTION()
		void LoadStage();
	
	UFUNCTION(BlueprintCallable)
		class ATileBase* GetStage(int32 XPosition, int32 YPosition);

private:
	UPROPERTY(VisibleAnywhere)
		TArray<class ATileBase*> Stages;

	UPROPERTY(VisibleAnywhere)
		class ATileBase* CurrentTile;

	UPROPERTY(VisibleAnywhere)
		class ATileBase* UnloadTile;
	
	UPROPERTY(VisibleAnywhere)
		EDirection MoveDir;

	//
public:
	UFUNCTION()
		void CompleteLoad();

	UFUNCTION()
		void CompleteUnLoad();

public:

		//FScriptDelegate UnloadDelegate;
//public:
//	UPROPERTY(VisibleAnywhere)
//		FScriptDelegate MyScriptDelegate;


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
