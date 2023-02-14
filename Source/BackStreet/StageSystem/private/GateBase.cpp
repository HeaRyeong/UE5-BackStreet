// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/GateBase.h"
#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../public/ChapterManagerBase.h"
#include "../public/ALevelScriptInGame.h"
#include "../public/StageManagerBase.h"
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
	//GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	InGameScriptRef =  Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
}

void AGateBase::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("Player"))
	{
		if (IsValid(TravelSequencePlayer))
		{
			TravelSequencePlayer->Play();
		}

		UpdateNewTile();

		////새 타일을 CrossFade 도중에 업데이트 한다.
		//GetWorldTimerManager().SetTimer(TravelSequenceDelayHandle, this, &AGateBase::UpdateNewTile, 1.0f, false, 0.5f);

		////CrossFade가 끝나면 Gate를 반환한다.
		//GetWorldTimerManager().SetTimer(ResourceReturnTimerHandle, FTimerDelegate::CreateLambda([&]() {
		//	GamemodeRef->ChapterManager->GetStageManager()->UnLoadStage();
		//	ClearAllTimerHandle();
		//	Destroy();
		//}), 1.0f, false, 0.75f);
				
	}
}

void AGateBase::InitGate()
{
	if (!bIsInit)
	{
		InitTileTravelSequence();
		CheckHaveToActive();
		bIsInit = true;
	}
}

void AGateBase::UpdateNewTile()
{
	if (!(this->Tags[0].Compare(FName(TEXT("StartGate")))))
	{
		InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_Start);
		UE_LOG(LogTemp, Log, TEXT("Start Gate"));
	}
	else if (!(this->Tags[0].Compare(FName(TEXT("ChapterGate")))))
	{
		if (InGameScriptRef->ChapterManager->IsChapterClear())
		{
			InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_Chapter);
			UE_LOG(LogTemp, Log, TEXT("ChapterGate"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Please Clear Mission"));
		}
	
	}
	else if (!(this->Tags[1].Compare(FName(TEXT("UP")))))
	{
		InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_UP);
		UE_LOG(LogTemp, Log, TEXT("Up Gate"));
	}
	else if (!(this->Tags[1].Compare(FName(TEXT("DOWN")))))
	{
		InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_DOWN);
		UE_LOG(LogTemp, Log, TEXT("Down Gate"));
	}
	else if (!(this->Tags[1].Compare(FName(TEXT("RIGHT")))))
	{
		InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_RIGHT);
		UE_LOG(LogTemp, Log, TEXT("Right Gate"));
	}
	else if (!(this->Tags[1].Compare(FName(TEXT("LEFT")))))
	{
		InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_LEFT);
		UE_LOG(LogTemp, Log, TEXT("Left Gate"));
	}
	
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
	ATileBase* tile = InGameScriptRef->ChapterManager->GetStageManager()->GetCurrentStage();

	if (this->Tags[0].IsEqual(FName(TEXT("StartGate"))))
	{
		return;
	}
	else if (this->Tags[0].IsEqual(FName(TEXT("ChapterGate"))))
	{
		if (tile->GetStageType() != EStageCategoryInfo::E_Boss)
		{
			Destroy();
		}
			
	}
	else
	{
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

}
