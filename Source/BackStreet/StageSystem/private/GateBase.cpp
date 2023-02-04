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

// Called when the game starts or when spawned
void AGateBase::BeginPlay()
{
	Super::BeginPlay();

	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	CurrTileRef = GamemodeRef->CurrentTile; // 이동하기전 스테이지 ( 이제 언로드 시킬 타일 )

	InitTileTravelSequence();
	CheckHaveToActive();
}

void AGateBase::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (IsValid(TravelSequencePlayer))
		{
			TravelSequencePlayer->Play();
		}

		//새 타일을 CrossFade 도중에 업데이트 한다.
		GetWorldTimerManager().SetTimer(TravelSequenceDelayHandle, this, &AGateBase::UpdateNewTile, 1.0f, false, 0.5f);

		//CrossFade가 끝나면 Gate를 반환한다.
		GetWorldTimerManager().SetTimer(ResourceReturnTimerHandle, FTimerDelegate::CreateLambda([&]() {
			CurrTileRef->UnLoadLevel();
			ClearAllTimerHandle();
			Destroy();
		}), 1.0f, false, 0.75f);
	}
}


void AGateBase::UpdateNewTile()
{
	UpdateGateInfo();
	ATileBase* nextTile = GamemodeRef->CurrentTile;
	nextTile->LoadLevel();
}

ULevelStreaming* AGateBase::UpdateGateInfo()
{
	// game mode 받아오기
	ULevelStreaming* ToUnLoadLevel = CurrTileRef->LevelRef;

	if (!(this->Tags[1].Compare(FName(TEXT("UP")))))
	{
		GamemodeRef->NextStage((uint8)EDirection::E_UP);
		UE_LOG(LogTemp, Log, TEXT("Up Gate"));
	}
	if (!(this->Tags[1].Compare(FName(TEXT("DOWN")))))
	{
		GamemodeRef->NextStage((uint8)EDirection::E_DOWN);
		UE_LOG(LogTemp, Log, TEXT("Down Gate"));
	}
	if (!(this->Tags[1].Compare(FName(TEXT("RIGHT")))))
	{
		GamemodeRef->NextStage((uint8)EDirection::E_RIGHT);
		UE_LOG(LogTemp, Log, TEXT("Right Gate"));
	}
	if (!(this->Tags[1].Compare(FName(TEXT("LEFT")))))
	{
		GamemodeRef->NextStage((uint8)EDirection::E_LEFT);
		UE_LOG(LogTemp, Log, TEXT("Left Gate"));
	}

	return ToUnLoadLevel;
}

void AGateBase::InitTileTravelSequence()
{
	if (!IsValid(TileTravelEffectSequence)) return;
	ALevelSequenceActor* outActor = nullptr;
	TravelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), TileTravelEffectSequence
		, FMovieSceneSequencePlaybackSettings(), outActor);
}

void AGateBase::ClearAllTimerHandle()
{
	GetWorldTimerManager().ClearTimer(TravelSequenceDelayHandle);
	GetWorldTimerManager().ClearTimer(ResourceReturnTimerHandle);
}

void AGateBase::CheckHaveToActive()
{
	ATileBase* tile = GamemodeRef->CurrentTile;

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

