// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

/* ----- 필수 라이브러리 ------------ */
#include "EngineMinimal.h"
#include "CollisionQueryParams.h"
#include "Perception/AIPerceptionTypes.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "TimerManager.h"

/* ----- Niagara 및 사운드 --------*/
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

/* ----- 구조체/Enum ------------ */
#include "../../Character/public/CharacterInfoStruct.h"
#include "../../Character/public/EnemyStatInfoStruct.h"
#include "../../StageSystem/public/DirectionEnumInfo.h"
#include "../../StageSystem/public/StageInfoStruct.h"
#include "../../Item/public/ItemInfoStruct.h"
#include "../../Item/public/ItemDataAssetInfo.h"
#include "../../Item/public/WeaponInfoStruct.h"
#include "../../Item/public/ProjectileInfoStruct.h"