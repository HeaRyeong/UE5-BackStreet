// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/GateBase.h"
#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../public/ChapterManagerBase.h"
#include "../public/ALevelScriptInGame.h"
#include "../public/StageManagerBase.h"
#include "Math/Color.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGateBase::AGateBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	OverlapVolume->SetupAttachment(RootComponent);
	Mesh->SetupAttachment(RootComponent);

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
	InGameScriptRef =  Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AGateBase::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//if (OtherActor->ActorHasTag("Player"))
	//{
	//	if (IsValid(TravelSequencePlayer))
	//	{
	//		TravelSequencePlayer->Play();
	//	}

	//	UpdateNewTile();

	//	////�� Ÿ���� CrossFade ���߿� ������Ʈ �Ѵ�.
	//	//GetWorldTimerManager().SetTimer(TravelSequenceDelayHandle, this, &AGateBase::UpdateNewTile, 1.0f, false, 0.5f);

	//	////CrossFade�� ������ Gate�� ��ȯ�Ѵ�.
	//	//GetWorldTimerManager().SetTimer(ResourceReturnTimerHandle, FTimerDelegate::CreateLambda([&]() {
	//	//	GamemodeRef->ChapterManager->GetStageManager()->UnLoadStage();
	//	//	ClearAllTimerHandle();
	//	//	Destroy();
	//	//}), 1.0f, false, 0.75f);
	//			
	//}
}

void AGateBase::InitGate()
{
	CheckHaveToActive();
}

void AGateBase::EnterGate()
{
	UpdateNewTile();
}

void AGateBase::ActiveGate()
{
	if (this->ActorHasTag(FName("ChapterGate")))
	{
		if (InGameScriptRef->ChapterManager->IsChapterClear())
		{
			UE_LOG(LogTemp, Log, TEXT("Call ActiveGate!"));
			Mesh->SetMaterial(0, GateMaterialList[1]);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Check IsChpaterClear!"));
		}
	}
	
}


void AGateBase::UpdateNewTile()
{
	if (this->ActorHasTag(FName("StartGate")))
	{
		InGameScriptRef->FadeOutDelegate.Broadcast();

		GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
	
				InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_Start);
				GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			}), 1.0f, false, 1.0f);
		//InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_Start);
		
	}
	else if (this->ActorHasTag(FName("ChapterGate")))
	{
		if (InGameScriptRef->ChapterManager->IsChapterClear())
		{
			InGameScriptRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
			InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_Chapter);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
		}else
		{
			GamemodeRef->PrintSystemMessageDelegate.Broadcast(FName(TEXT("���� �̼��� �����ֽ��ϴ�, �̼��� Ŭ���� ���ּ���.")), FColor::White);
		}
	}
	else
	{
		 if (this->ActorHasTag(FName("UP")))
		{
			InGameScriptRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
			InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_UP);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Up Gate"));
		}
		else if (this->ActorHasTag(FName("DOWN")))
		{
			InGameScriptRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
			InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_DOWN);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Down Gate"));
		}
		else if (this->ActorHasTag(FName("RIGHT")))
		{
			InGameScriptRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
			InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_RIGHT);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Right Gate"));
		}
		else if (this->ActorHasTag(FName("LEFT")))
		{
			InGameScriptRef->FadeOutDelegate.Broadcast();
			GetWorldTimerManager().SetTimer(FadeOutEffectHandle, FTimerDelegate::CreateLambda([&]() {
			InGameScriptRef->ChapterManager->GetStageManager()->MoveStage((uint8)EDirection::E_LEFT);
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
				}), 1.0f, false, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Left Gate"));
		}

	}
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
		// Set Material	
		if (InGameScriptRef->ChapterManager->IsChapterClear())
		{
			Mesh->SetMaterial(0, GateMaterialList[1]);
		}
		else
		{
			Mesh->SetMaterial(0, GateMaterialList[2]);
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

		// Set Material
		Mesh->SetMaterial(0, GateMaterialList[0]);
	}

}
