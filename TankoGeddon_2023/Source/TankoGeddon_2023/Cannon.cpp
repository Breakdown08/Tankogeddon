#include "Cannon.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = sceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(sceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::Fire()
{
	if (!IsReadyToFire() || Ammo <= 0)
	{
		return;
	}
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
	bIsReadyToFire = false;
	Ammo--;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReloadSpecial, 1 / FireRate, false);
	

}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::ReloadSpecial()
{
	bIsReadyToFire = true;
	if (projectileQueue <= BurstFireCount)
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
	projectileQueue += 1;
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile %hhu"), Ammo));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire trace %hhu"), Ammo));
	}
	if (projectileQueue == BurstFireCount)
	{
		projectileQueue = 0;
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}



