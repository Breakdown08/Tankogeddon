#pragma once

#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "Engine/TargetPoint.h"
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

	UFUNCTION()
	FVector GetTurretForwardVector();
	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition);
	
	UFUNCTION()
	float GetMovementAccurency() { return MovementAccurency; }

	FVector GetEyesPosition();
	
	bool IsPlayerSeen(AUnit* Spectator, AUnit* Player);

	float GetHealth();
	float GetMaxHealth();
	
protected:
	void Die();
	void DamageTaked(float Value);
	void BeginPlay() override;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MovementAccurency = 50.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* TakeDamageAudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* TakeDamageParticleEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* DieAudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DieParticleEffect;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthWidgetComponent;
	
	UPROPERTY()
	ACannon* Cannon;
	
private:
	float TurretRotationInterpolationKey = 0.5f;
	void SelfDestroy() { this->Destroy(); }
	FTimerHandle DieTimer;
};
