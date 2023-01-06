// Copyright Epic Games, Inc. All Rights Reserved.


#include "../public/BackStreetGameModeBase.h"

void ABackStreetGameModeBase::PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius)
{
	if (CameraShakeEffectList.Num() < (uint8)EffectType) return;

	UE_LOG(LogTemp, Warning, TEXT("BOOM"));

	Location = Location + FVector(-700.0f, 0.0f, 1212.0f); //캐릭터의 Camera의 위치에 맞게 변환
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeEffectList[(uint8)EffectType], Location, Radius * 0.75f, Radius);
}
