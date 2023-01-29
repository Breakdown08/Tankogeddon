#include "Cannon.h"
#include "DamageTaker.h"
#include "Projectile.h"
#include "Unit.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = sceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(sceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
	AudioEffect->SetupAttachment(ProjectileSpawnPoint);
}

void ACannon::Fire()
{
	if (!IsReadyToFire() || Ammo <= 0)
	{
		return;
	}
	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	/*if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ShootForceEffect)
		{
			FForceFeedbackParameters shootForceEffectParams;
			shootForceEffectParams.bLooping = false;
			shootForceEffectParams.Tag = "shootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
		}
		if (ShootShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootShake);
		}
	}*/
	
	bIsSpecialFireMode = false;
	bIsReadyToFire = false;
	Ammo--;
	InitFire();
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}


void ACannon::FireSpecial()
{
	if (!IsReadyToFire() || Ammo <= 0)
	{
		return;
	}
	bIsSpecialFireMode = true;
	bIsReadyToFire = false;
	Ammo--;
	InitFire();
	ReloadSpecial();
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::ReloadSpecial()
{
	if (projectileQueue <= BurstFireCount - 1)
	{
		GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACannon::InitFire, 1 / BurstFireRate, true, 0.0f);
	}
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::InitFire()
{
	if (bIsSpecialFireMode)
	{
		projectileQueue += 1;
	}
	
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile %hhu"), Ammo));
		SpawnProjectile();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire trace %hhu"), Ammo));
		SpawnTrace();
	}
	if (projectileQueue == BurstFireCount)
	{
		projectileQueue = 0;
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
	}
}

void ACannon::SpawnProjectile()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), spawnParams);
	if (projectile)
	{
		projectile->Start();
	}
}

void ACannon::SpawnTrace()
{
	FHitResult hitResult;
	FCollisionQueryParams traceParams;
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;

	FVector Start = ProjectileSpawnPoint->GetComponentLocation();
	FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECC_GameTraceChannel1, traceParams))
	{
		DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Purple, false, 1.0f, 0, 2.0f);
		if (hitResult.GetActor())
		{
			IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(hitResult.GetActor());
			if (DamageTakerActor)
			{
				FDamageData damageData;
				damageData.DamageValue = 1.0f;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;

				DamageTakerActor->TakeDamage(damageData);
			}
			else
			{
				hitResult.GetActor()->Destroy();
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	}
}

void ACannon::AddAmmo(uint8 Value)
{
	Ammo+= Value;
}

void ACannon::SetDirectionProjectilePoint(APawn* TargetPawn)
{
	FRotator currentRotation = CannonMesh->GetComponentRotation();
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(ProjectileSpawnPoint->GetComponentLocation(), TargetPawn->GetActorLocation());
	targetRotation.Roll = currentRotation.Roll;
	targetRotation.Yaw = currentRotation.Yaw;
	ProjectileSpawnPoint->SetWorldRotation(FMath::Lerp(currentRotation, targetRotation, 1));
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}
