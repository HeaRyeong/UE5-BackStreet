// Copyright Epic Games, Inc. All Rights Reserved.


#include "../public/BackStreetGameModeBase.h"

void ABackStreetGameModeBase::PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius)
{
	if (CameraShakeEffectList.Num() < (uint8)EffectType) return;

	UE_LOG(LogTemp, Warning, TEXT("BOOM"));

	Location = Location + FVector(-700.0f, 0.0f, 1212.0f); //ĳ������ Camera�� ��ġ�� �°� ��ȯ
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeEffectList[(uint8)EffectType], Location, Radius * 0.75f, Radius);
}
