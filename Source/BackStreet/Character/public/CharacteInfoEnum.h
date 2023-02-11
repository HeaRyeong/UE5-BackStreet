#pragma once

UENUM(BlueprintType)
enum class ECharacterDebuffType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Flame				UMETA(DisplayName = "Flame"),
	E_Poison			UMETA(DisplayName = "Poison"),
	E_AttackDown		UMETA(DisplayName = "AttackDown"),
	E_DefenseDown		UMETA(DisplayName = "DefenseDown"),
	E_Slow				UMETA(DisplayName = "Slow"),
	E_Stun				UMETA(DisplayName = "Stun"),
};

UENUM(BlueprintType)
enum class ECharacterBuffType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Healing			UMETA(DisplayName = "Healing"),
	E_AttackUp			UMETA(DisplayName = "AttackUp"),
	E_DefenseUp			UMETA(DisplayName = "DefenseUp"),
	E_SpeedUp			UMETA(DisplayName = "SpeedUp"),
	E_Invincibility		UMETA(DisplayName = "Invincibility"),
	E_Infinite			UMETA(DisplayName = "Infinite")
};

UENUM(BlueprintType)
enum class EAIBehaviorType : uint8
{
	E_Idle			UMETA(DisplayName = "Idle"),
	E_Patrol		UMETA(DisplayName = "Patrol"),
	E_Detect		UMETA(DisplayName = "Detect"),
	E_Chase			UMETA(DisplayName = "Chase"),
	E_Attack		UMETA(DisplayName = "Attack"),
	E_Return		UMETA(DisplayName = "Return"),
	E_Stun			UMETA(DisplayName = "Stun")
};

UENUM(BlueprintType)
enum class ECharacterActionType : uint8
{
	E_Idle			UMETA(DisplayName = "Idle"),
	E_Attack		UMETA(DisplayName = "Attack"),
	E_Defense		UMETA(DisplayName = "Defense"),
	E_Roll			UMETA(DisplayName = "Roll"),
	E_Jump			UMETA(DisplayName = "Jump"),
	E_Reload		UMETA(DisplayName = "Reload"),
	E_Stun			UMETA(DisplayName = "Stun"),
	E_Die			UMETA(DisplayName = "Die")
};