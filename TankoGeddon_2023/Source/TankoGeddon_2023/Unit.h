#pragma once

#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "GameFramework/Pawn.h"
#include "Unit.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()

class TANKOGEDDON_2023_API AUnit : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	AUnit();

	UFUNCTION()
	void TakeDamage(FDamageData DamageData) override;
	void AddAmmo(uint8 Value);
	void Fire();

protected:
	
	void Die();
	void DamageTaked(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;
	
	UPROPERTY()
	ACannon* Cannon;
};
