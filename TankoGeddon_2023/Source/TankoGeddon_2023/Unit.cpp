#include "Unit.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "MyGameInstance.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Tasks/GameplayTask_WaitDelay.h"

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

	TakeDamageParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TakeDamageEffect"));
	TakeDamageParticleEffect->SetupAttachment(BodyMesh);

	TakeDamageAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDamageAudioEffect"));
	TakeDamageAudioEffect->SetupAttachment(BodyMesh);

	DieParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DieEffect"));
	DieParticleEffect->SetupAttachment(BodyMesh);

	DieAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudioEffect"));
	DieAudioEffect->SetupAttachment(BodyMesh);
}

void AUnit::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
	TakeDamageParticleEffect->ActivateSystem();
	TakeDamageAudioEffect->Play();
}


void AUnit::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

FVector AUnit::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void AUnit::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator currentTurretRotation = TurretMesh->GetComponentRotation();
	
	targetRotation.Pitch = currentTurretRotation.Pitch;
	targetRotation.Roll = currentTurretRotation.Roll;
	
	TurretMesh->SetWorldRotation(FMath::Lerp(currentTurretRotation, targetRotation, TurretRotationInterpolationKey));
}

FVector AUnit::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}




void AUnit::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	DieParticleEffect->ActivateSystem();
	DieAudioEffect->Play();

	if (IsPlayerControlled())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>( GetGameInstance());
		if (MyGameInstance)
		{
			MyGameInstance->ShowWidget();
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(DieTimer, this, &AUnit::SelfDestroy, 1, false);

	
}

void AUnit::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Unit %s taked damage: %f, health: %f"), *GetName(), Value, HealthComponent->GetHealth());
}

bool AUnit::IsPlayerSeen(AUnit* Spectator, AUnit* Player)
{
	FVector playerPos = Player->GetActorLocation();
	FVector eyesPos = Spectator->GetEyesPosition();

	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);

	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(Spectator);
	traceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECC_Visibility, traceParams))
	{
		if (hitResult.GetActor())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
			return hitResult.GetActor() == Player;
		}
	}
	DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
	return false;
}


void AUnit::AddAmmo(uint8 Value)
{
	if (Cannon)
	{
		Cannon->AddAmmo(Value);
	}
}
