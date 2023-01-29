#include "TankFactory.h"

#include "HealthComponent.h"
#include "NavigationSystemTypes.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(sceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon Setup Point"));
	TankSpawnPoint->AttachToComponent(sceneComp, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);

	SpawnAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SpawnAudioEffect"));
	SpawnAudioEffect->SetupAttachment(BuildingMesh);

	DieAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudioEffect"));
	DieAudioEffect->SetupAttachment(BuildingMesh);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _targetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this,
	&ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	newTank->SetPatrollingPoints(TankWayPoints);
	newTank->SpawnDefaultController();
	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
	SpawnAudioEffect->Play();
}

void ATankFactory::Die()
{
	DieAudioEffect->Play();
	GetWorld()->GetTimerManager().SetTimer(DieTimer, this, &ATankFactory::SelfDestroy, 1, false);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"),
	*GetName(), DamageValue, HealthComponent->GetHealth());
}


void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

