// Copyright Epic Games, Inc. All Rights Reserved.


#include "../public/BackStreetGameModeBase.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void ABackStreetGameModeBase::InitGame()
{

	for (ALevelStreamerActor* entity : TActorRange<ALevelStreamerActor>(GetWorld()))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, entity->GetName());

		if (entity->GetName() == FString(TEXT("LevelStreamerActor_1")))
		{
			StreamerVolumeA = entity;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CurrentStreamerVolume:"));
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, StreamerVolumeA->GetName());

		}
		if (entity->GetName() == FString(TEXT("LevelStreamerActor_2")))
		{
			StreamerVolumeB = entity;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NextStreamerVolume:"));
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, StreamerVolumeB->GetName());

		}

	}

	Map.CreateMaze(5, 5, 1);
	CurrentTransform = FVector(0.f, 0.f, 0.f);
	PreTransform = FVector(0.f, -5000.f, 0.f);
	CurrentVolumeLocation = FVector(0.f, 0.f, 0.f);
	CurrentVolume = StreamerVolumeA;

	ATile* currentTile = Map.GetCurrentTile();
	currentTile->down = true;

	SetMap();
	SetVolume();
}


void ABackStreetGameModeBase::SelectRandomMap()
{
	int NextMap = FMath::RandRange(1, 3);

	switch (NextMap)
	{
	case 1:
		NextLevelToLoad = FName(TEXT("Main1"));
		break;
	case 2:
		NextLevelToLoad = FName(TEXT("Main2"));
		break;
	case 3:
		NextLevelToLoad = FName(TEXT("Main3"));
		break;
		//case 4:
		//	NextLevelToLoad = FName(TEXT("Sub1Prefab"));
		//	break;
		//case 5:
		//	NextLevelToLoad = FName(TEXT("Sub2Prefab"));
		//	break;
		//case 6:
		//	NextLevelToLoad = FName(TEXT("Sub3Prefab"));
		//	break;
	}
}

void ABackStreetGameModeBase::SetMap()
{
	SelectRandomMap();
	ATile* currentTile = Map.GetCurrentTile();
	UWorld* world = GetWorld();

	PrintTileInfo();
	{
		UObject* SpawnActor;
		FVector spawnLocation;

		if (currentTile->up)
		{
			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Gate")));
			spawnLocation = CurrentTransform + FVector(-1500, 2450, 1000);
		}
		else
		{
			spawnLocation = CurrentTransform + FVector(0, 2450, 0);
			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Wall")));
		}

		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		if (!SpawnActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
			return;
		}

		UClass* SpawnClass = SpawnActor->StaticClass();
		if (SpawnClass == NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
			return;
		}

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator rotator = FRotator(0, 0, 0);

		World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, spawnLocation, rotator, SpawnParams);

	}

	{
		UObject* SpawnActor;
		FVector spawnLocation;
		if (currentTile->down)
		{

			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Gate")));
			spawnLocation = CurrentTransform + FVector(-1500, -2450, 1000);
		}
		else
		{
			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Wall")));
			spawnLocation = CurrentTransform + FVector(0, -2450, 0);
		}

		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		if (!SpawnActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
			return;
		}

		UClass* SpawnClass = SpawnActor->StaticClass();
		if (SpawnClass == NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
			return;
		}

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator rotator = FRotator(0, 0, 0);
		World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, spawnLocation, rotator, SpawnParams);

	}

	{
		UObject* SpawnActor;
		FVector spawnLocation;
		if (currentTile->left)
		{
			// 해당 위치에 Gate 생성
			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Gate")));
			spawnLocation = CurrentTransform + FVector(2450, -1500, 1000);
		}
		else
		{
			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Wall")));
			spawnLocation = CurrentTransform + FVector(2450, 0, 0);
		}

		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		if (!SpawnActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
			return;
		}

		UClass* SpawnClass = SpawnActor->StaticClass();
		if (SpawnClass == NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
			return;
		}

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator rotator = FRotator(0, 90, 0);
		World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, spawnLocation, rotator, SpawnParams);

	}

	{
		UObject* SpawnActor;
		FVector spawnLocation = CurrentTransform + FVector(-2450, 0, 0);
		if (currentTile->right)
		{
			// 해당 위치에 Gate 생성
			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Gate")));
			spawnLocation = CurrentTransform + FVector(-2450, -1500, 1000);
		}
		else
		{
			SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/ThirdPerson/Blueprints/BP_Wall")));
			spawnLocation = CurrentTransform + FVector(-2450, 0, 0);
		}

		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		if (!SpawnActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
			return;
		}

		UClass* SpawnClass = SpawnActor->StaticClass();
		if (SpawnClass == NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
			return;
		}

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator rotator = FRotator(0, 90, 0);

		World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, spawnLocation, rotator, SpawnParams);

	}


}


void ABackStreetGameModeBase::StageClear()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stage Clear!"));


	GateOpen();

}

void ABackStreetGameModeBase::GateOpen()
{
	for (AStaticMeshActor* entity : TActorRange<AStaticMeshActor>(GetWorld()))
	{
		if (entity->ActorHasTag(TEXT("Gate")))
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Find Gate! Now Open it"));
			FVector GateLoc = entity->GetActorLocation();
			GateLoc += FVector(0, 0, 1000);
			entity->SetActorLocation(GateLoc);
		}

	}

}

void ABackStreetGameModeBase::SetVolume()
{
	ATile* currentTile = Map.GetCurrentTile();
	if (CurrentTransform != FVector(0.f, 0.f, 0.f))
	{

		if (CurrentVolume == StreamerVolumeA)
		{
			CurrentVolume = StreamerVolumeB;
		}
		else
		{
			CurrentVolume = StreamerVolumeA;
		}
		// 다음 이동 위치로 볼륨과 CurrentTransform 설정하기

		CurrentVolume->SetActorLocation(CurrentVolumeLocation);
	}

	if (CurrentTransform.X > PreTransform.X)
	{
		// 이전 스테이지는 오른쪽

	}
	else if (CurrentTransform.X < PreTransform.X)
	{
		// 이전 스테이지는 왼쪽

	}
	else if (CurrentTransform.Y > PreTransform.Y)
	{
		// 이전 스테이지는 아래
	}
	else if (CurrentTransform.Y < PreTransform.Y)
	{
		// 이전 스테이지는 위
	}

	CurrentVolume->SetLevelToLoad(NextLevelToLoad, CurrentTransform, true);
}

void ABackStreetGameModeBase::PrintTileInfo()
{
	ATile* currentTile = Map.GetCurrentTile();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Up"));
	if (currentTile->up)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Gate"));
	}
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Wall"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Down"));
	if (currentTile->down)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Gate"));
	}
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Wall"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("left"));
	if (currentTile->left)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Gate"));
	}
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Wall"));


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Right"));
	if (currentTile->right)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Gate"));
	}
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Wall"));
}
