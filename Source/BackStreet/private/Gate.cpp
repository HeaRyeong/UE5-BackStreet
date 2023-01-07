// Fill out your copyright notice in the Description page of Project Settings.


#include "Gate.h"
#include "BackStreetGameModeBase.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGate::AGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	OverlapVolume->SetupAttachment(RootComponent);

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGate::OverlapBegins);
}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();
	CheckHaveToActive();
	
}

// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


ULevelStreaming* AGate::UpdateGateInfo()
{
	// game mode 받아오기
	ABackStreetGameModeBase* mode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ULevelStreaming* ToUnLoadLevel = mode->CurrTile->LevelRef;

	if (!(this->Tags[1].Compare(FName(TEXT("UP")))))
	{
		mode->NextStage((uint8)EDirection::E_UP);
		UE_LOG(LogTemp, Log, TEXT("Up Gate"));
	}
	if (!(this->Tags[1].Compare(FName(TEXT("DOWN")))))
	{
		mode->NextStage((uint8)EDirection::E_DOWN);
		UE_LOG(LogTemp, Log, TEXT("Down Gate"));
	}
	if (!(this->Tags[1].Compare(FName(TEXT("RIGHT")))))
	{
		mode->NextStage((uint8)EDirection::E_RIGHT);
		UE_LOG(LogTemp, Log, TEXT("Right Gate"));
	}
	if (!(this->Tags[1].Compare(FName(TEXT("LEFT")))))
	{
		mode->NextStage((uint8)EDirection::E_LEFT);
		UE_LOG(LogTemp, Log, TEXT("Left Gate"));
	}

	return ToUnLoadLevel;
}


void AGate::CheckHaveToActive()
{
	ABackStreetGameModeBase* mode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ATile* tile = mode->CurrTile;

	for (int i = 0; i < 4; i++)
	{
		if (!tile->Gate[i])
		{
			switch (i)
			{
			case 0:
				if (this->Tags[1].IsEqual(FName(TEXT("UP"))))
					Destroy();
				break;
			case 1:
				if (this->Tags[1].IsEqual(FName(TEXT("DOWN"))))
					Destroy();
				break;
			case 2:
				if (this->Tags[1].IsEqual(FName(TEXT("LEFT"))))
					Destroy();
				break;
			case 3:
				if (this->Tags[1].IsEqual(FName(TEXT("RIGHT"))))
					Destroy();
				break;
			default:
				break;
			}

		}
	}

}

void AGate::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ABackStreetGameModeBase* mode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		ATile* UnLoadTile = mode->CurrTile; // 이동하기전 스테이지 ( 이제 언로드 시킬 타일 )

		UpdateGateInfo();
		ATile* LoadTile = mode->CurrTile;
		LoadTile->LoadLevel();
		UnLoadTile->UnLoadLevel();
		Destroy();

	}
}