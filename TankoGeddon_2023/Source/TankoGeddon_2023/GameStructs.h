#pragma once

#include "CoreMinimal.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use trace"),
};

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	MainMenu = 0 UMETA(DisplayName = "Show Main menu"),
	GameOver = 1 UMETA(DisplayName = "Show Game over"),
};

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageValue;

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	AActor* DamageMaker;	
};

UCLASS()
class TANKOGEDDON_2023_API UGameStructs : public UObject
{
	GENERATED_BODY()
};
