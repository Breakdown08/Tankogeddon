#include "Cannon.h"

#include "Projectile.h"
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
			UE_LOG(LogTemp, Warning, TEXT("Trace overlap : %s"), *hitResult.GetActor()->GetName());
			hitResult.GetActor()->Destroy();
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

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}



