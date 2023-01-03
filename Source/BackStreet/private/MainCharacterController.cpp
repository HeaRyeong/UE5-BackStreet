// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterController.h"
#include "../public/MainCharacterBase.h"
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
	auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
	{
		//return GetRightAnalogRotation();
		return FRotator();
	}
	return GetRotationToCursor();
}

FRotator AMainCharacterController::GetRotationToCursor()
{
	FRotator retRotation;
	
	//Trace�� ������ �÷��̾��� FollowingCamera ��ġ
	FVector traceStartLocation = PlayerRef->FollowingCamera->GetComponentLocation();
	FVector traceEndLocation, mouseDirection;
	FHitResult hitResult;

	//���콺 Ŀ�� ��ġ���� �ٴ������� 5000��ŭ�� ��ġ�� Trace�� ������ ����
	DeprojectMousePositionToWorld(traceEndLocation, mouseDirection);
	traceEndLocation += (mouseDirection * 5000.0f);

	//ī�޶󿡼� Ŀ���� �ٴ� ��ġ���� LineTrace�� ���� -> ���� Ŀ���� ���� ��ȣ�ۿ� ��ġ�� hitResult.Location�� ���
	GetWorld()->LineTraceSingleByChannel(hitResult, traceStartLocation, traceEndLocation
		, ECollisionChannel::ECC_Visibility);

	//hit�� �����ߴٸ�
	if (hitResult.bBlockingHit)
	{
		retRotation = UKismetMathLibrary::FindLookAtRotation(PlayerRef->GetMesh()->GetComponentLocation(), hitResult.Location);
		retRotation = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, retRotation.Yaw);
		return retRotation;
	}

	return FRotator();
}
