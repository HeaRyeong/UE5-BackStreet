#pragma once

#include "Engine/DataTable.h"
#include "../../Character/public/CharacteInfoEnum.h"
#include "ProjectileInfoStruct.generated.h"


USTRUCT(BlueprintType)
struct FProjectileStatStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	//�߻�ü�� ID
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		uint8 ProjectileID;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FName ProjectileName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FName Description;

	//���� Ÿ���� �߻�ü����? (RadialDamage)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsExplosive;

	//�߻�ü�� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileSpeed = 2000.0f;

	//�߻�ü�� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileDamage = 0.2f;

	//�߷� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float GravityScale = 1.0f;

	//������ �Ǵ���?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsHoming = false;

	//�߻�ü�� �� �ϳ��� ������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;
};