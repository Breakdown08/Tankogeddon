#include "Unit.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &AUnit::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &AUnit::DamageTaked);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);
}

void AUnit::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}


void AUnit::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void AUnit::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

void AUnit::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Unit %s taked damage: %f, health: %f"), *GetName(), Value, HealthComponent->GetHealth());
}

void AUnit::AddAmmo(uint8 Value)
{
	if (Cannon)
	{
		Cannon->AddAmmo(Value);
	}
}