// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "StageData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateAIContorl);

UCLASS()
class BACKSTREET_API AStageData : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateAIContorl AIOnDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateAIContorl AIOffDelegate;

public:
	AStageData();

	UFUNCTION()
		int32 GetXPos() { return XPos; }

	UFUNCTION()
		void SetXPos(int32 Pos) { XPos = Pos; }

	UFUNCTION()
		int32 GetYPos() { return YPos; }

	UFUNCTION()
		void SetYPos(int32 Pos) { YPos = Pos; }

	UFUNCTION()
		FVector GetStageLocation() { return StageLocation; }

	UFUNCTION()
		void SetStageLocation(FVector Vector) { StageLocation = Vector; }

	UFUNCTION(BlueprintPure)
		EStageCategoryInfo GetStageType() { return StageType; }

	UFUNCTION()
		void SetStageType(EStageCategoryInfo Type) { StageType = Type; }

	UFUNCTION()
		FName GetLevelToLoad() { return LevelToLoad; }

	UFUNCTION()
		void SetLevelToLoad(FName Level) { LevelToLoad = Level; }

	UFUNCTION()
		bool GetIsClear() { return bIsClear; }

	UFUNCTION()
		void SetIsClear(bool Clear) {  bIsClear = Clear; }

	UFUNCTION()
		bool GetIsValid() { return bIsValid; }

	UFUNCTION()
		void SetIsValid(bool Valid) { bIsValid = Valid; }

	UFUNCTION(BlueprintCallable)
		bool GetIsVisited() { return bIsVisited; }

	UFUNCTION()
		void SetIsVisited(bool Visited) { bIsVisited = Visited; }

	UFUNCTION()
		void AddGateInfo(bool Target) { GateInfo.Add(Target); }

	UFUNCTION(BlueprintCallable)
		bool GetGateInfoDir(EDirection Dir) { return GateInfo[(uint8)(Dir)]; }

	UFUNCTION()
		void SetGateInfoDir(EDirection Dir, bool Info) {  GateInfo[(uint8)(Dir)] = Info; }

	UFUNCTION(BlueprintPure)
		TArray<FVector> GetMonsterSpawnPoints() { return MonsterSpawnPoints; }

	UFUNCTION(BlueprintPure)
		TArray<FVector> GetItemSpawnPoints() { return ItemSpawnPoints; }

	UFUNCTION(BlueprintPure)
		TArray<FVector> GetCharacterSpawnPoint() { return CharacterSpawnPoint; }

	UFUNCTION(BlueprintPure)
		TArray<FVector> GetRewardBoxSpawnPoint() { return RewardBoxSpawnPoint; }

	UFUNCTION()
		void AddMonsterSpawnPoint(FVector Target) { MonsterSpawnPoints.Add(Target); }

	UFUNCTION()
		void AddItemSpawnPoint(FVector Target) { ItemSpawnPoints.Add(Target); }

	UFUNCTION()
		void AddCharacterSpawnPoint(FVector Target) { CharacterSpawnPoint.Add(Target); }

	UFUNCTION()
		void AddRewardBoxSpawnPoint(FVector Target) { RewardBoxSpawnPoint.Add(Target); }

	UFUNCTION()
		class AEnemyCharacterBase* GetMonsterIdx(int32 Idx) { return MonsterList[Idx]; }

	UFUNCTION()
		class AItemBase* GetItemIdx(int32 Idx) { return ItemList[Idx]; }

	UFUNCTION()
		class AItemBoxBase* GetItemBoxIdx(int32 Idx) { return ItemBoxList[Idx]; }

	UFUNCTION(BlueprintCallable)
		TArray<class AEnemyCharacterBase*> GetMonsterList() { return MonsterList; }

	UFUNCTION()
		TArray<class AItemBase*> GetItemList() { return ItemList; }

	UFUNCTION()
		TArray<class AItemBoxBase*> GetItemBoxList() { return ItemBoxList; }

	UFUNCTION()
		TArray<class AGateBase*> GetGateList() { return GateList; }

	UFUNCTION()
		class ARewardBoxBase* GetRewardBox() { return RewardBoxRef; }

	UFUNCTION()
		void SetRewardBox(class ARewardBoxBase* Target) {  RewardBoxRef = Target; }

	UFUNCTION()
		void AddMonsterList(class AEnemyCharacterBase* Target) { MonsterList.Add(Target); }

	UFUNCTION()
		void RemoveMonsterList(class AEnemyCharacterBase* Target) { MonsterList.Remove(Target); }

	UFUNCTION()
		void AddItemBoxList(class AItemBoxBase* Target) { ItemBoxList.Add(Target); }

	UFUNCTION()
		void RemoveItemBoxList(class AItemBoxBase* Target) { ItemBoxList.Remove(Target); }

	UFUNCTION()
		void AddItemList(class AItemBase* Target) { ItemList.Add(Target); }

	UFUNCTION()
		void AddGateList(class AGateBase* Target) { GateList.Add(Target); }

	UFUNCTION()
		void EmptyGateList() { GateList.Empty(); }

	UFUNCTION()
		ULevelStreaming* GetLevelRef() { return LevelRef; }

	UFUNCTION()
		void SetLevelRef(ULevelStreaming* Target) { LevelRef = Target; }
private:
	UPROPERTY()
		int32 XPos;

	UPROPERTY()
		int32 YPos;

	UPROPERTY()
		FVector StageLocation;

	UPROPERTY()
		TArray<bool> GateInfo;

	UPROPERTY()
		EStageCategoryInfo StageType;

	UPROPERTY()
		FName LevelToLoad;

	UPROPERTY()
		bool bIsVisited;

	UPROPERTY()
		bool bIsClear;

	UPROPERTY()
		bool bIsValid;

	UPROPERTY()
		TArray<FVector> MonsterSpawnPoints;

	UPROPERTY()
		TArray<FVector> ItemSpawnPoints;

	UPROPERTY()
		TArray<FVector> CharacterSpawnPoint;

	UPROPERTY()
		TArray<FVector> RewardBoxSpawnPoint;

	UPROPERTY()
		TArray<class AEnemyCharacterBase*> MonsterList;

	UPROPERTY()
		TArray<class AItemBase*> ItemList;

	UPROPERTY()
		TArray<class AItemBoxBase*> ItemBoxList;

	UPROPERTY()
		TArray<class AGateBase*> GateList;

	UPROPERTY()
		class ARewardBoxBase* RewardBoxRef;
	
	UPROPERTY()
		ULevelStreaming* LevelRef;

	UPROPERTY()
		FTimerHandle StageTimerHandle;

	UPROPERTY()
		int32 StageTime;
	
};
