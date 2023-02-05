// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/GateBase.h"
#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGateBase::AGateBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	OverlapVolume->SetupAttachment(RootComponent);

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGateBase::OverlapBegins);
}

// Called every frame
void AGateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGateBase::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ABackStreetGameModeBase* gamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		ATileBase* UnLoadTile = gamemodeRef->CurrentTile; // 이동하기전 스테이지 ( 이제 언로드 시킬 타일 )

		UpdateGateInfo();
		UpdateMiniMap();
		ATileBase* LoadTile = gamemodeRef->CurrentTile;
		LoadTile->LoadLevel();
		UnLoadTile->UnLoadLevel();
		Destroy();
	}
}


// Called when the game starts or when spawned
void AGateBase::BeginPlay()
{
	Super::BeginPlay();
	CheckHaveToActive();
}

ULevelStreaming* AGateBase::UpdateGateInfo()
{
	// game mode 받아오기
	ABackStreetGameModeBase* mode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ULevelStreaming* ToUnLoadLevel = mode->CurrentTile->LevelRef;

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

void AGateBase::UpdateMiniMap()
{
	
}

void AGateBase::CheckHaveToActive()
{
	ABackStreetGameModeBase* gamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	ATileBase* tile = gamemodeRef->CurrentTile;

	if (IsValid(tile))
	{
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
}

