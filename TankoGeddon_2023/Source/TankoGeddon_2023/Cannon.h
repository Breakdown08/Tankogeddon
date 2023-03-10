#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
//#include "Camera/CameraShakeBase.h",
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_2023_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();

	void Fire();
	void FireSpecial();
	void Reload();
	void ReloadSpecial();
	bool IsReadyToFire();
	void InitFire();
	void SpawnProjectile();
	void SpawnTrace();
	void AddAmmo(uint8 Value);
	void SetDirectionProjectilePoint(APawn* PlayerPawn);

	FTimerHandle ReloadTimer;
	FTimerHandle BurstTimer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float BurstFireRate = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	uint8 BurstFireCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float FireRange = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float FireDamage = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	uint8 Ammo = 10;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UForceFeedbackEffect* ShootForceEffect;
	
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UCameraShakeBase> ShootShake;

	
private:
	bool bIsReadyToFire = false;
	uint8 projectileQueue = 0;
	bool bIsSpecialFireMode = false;
};
