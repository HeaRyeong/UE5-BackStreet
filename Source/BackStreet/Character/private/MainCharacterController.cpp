// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterController.h"
#include "../public/MainCharacterBase.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetMathLibrary.h"

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

	this->bShowMouseCursor = true;

	PlayerRef = Cast<AMainCharacterBase>(GetCharacter());
}

FRotator AMainCharacterController::GetAimingRotation()
{
	//�ӽ� �ڵ�) ���� Attach���ڸ��� Gamemode�� ���� UI, Controller ������ Delegate Broadcast
	//auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	//if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
	//{
		//return GetRightAnalogRotation();
	//	return FRotator();
	//}
	return GetRotationToCursor();
}

FRotator AMainCharacterController::GetRotationToCursor()
{
	if (!IsValid(PlayerRef)) return FRotator();

	FRotator retRotation = FRotator();
	FVector cursorWorldDeprojectionLocation = GetCursorDeprojectionWorldLocation();

	retRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), cursorWorldDeprojectionLocation);
	retRotation = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, retRotation.Yaw + 270.0f);
	return retRotation;
}

FVector AMainCharacterController::GetCursorDeprojectionWorldLocation()
{
	FVector traceStartLocation, traceEndLocation, mouseDirection;
	FHitResult hitResult;

	//Trace�� ������ ���콺�� World Location
	DeprojectMousePositionToWorld(traceStartLocation, mouseDirection);

	//���콺 Ŀ�� ��ġ���� �ٴ������� INF��ŭ�� ��ġ�� Trace�� ������ ����
	traceEndLocation = traceStartLocation + mouseDirection * 250000.0f;

	//ī�޶󿡼� Ŀ���� �ٴ� ��ġ���� LineTrace�� ���� -> ���� Ŀ���� ���� ��ȣ�ۿ� ��ġ�� hitResult.Location�� ���
	GetWorld()->LineTraceSingleByChannel(hitResult, traceStartLocation, traceEndLocation
		, ECollisionChannel::ECC_Camera);

	if (hitResult.bBlockingHit) return hitResult.Location;
	return FVector();
}

bool AMainCharacterController::GetActionKeyIsDown(FName MappingName)
{
	TArray<FInputActionKeyMapping> actionKeyMappingList;
	UInputSettings::GetInputSettings()->GetActionMappingByName(MappingName, actionKeyMappingList);

	for (FInputActionKeyMapping& inputKey : actionKeyMappingList)
	{
		if (IsInputKeyDown(inputKey.Key))
		{
			return true;
		}
	}
	return false;
}
