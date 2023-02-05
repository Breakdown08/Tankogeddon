#include "Projectile.h"

#include "DamageTaker.h"
#include "Animation/AnimInstanceProxy.h"
#include "Components/SphereComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpehereCollision"));
	SphereCollision->SetupAttachment(SceneComp);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	SphereCollision->SetCollisionObjectType(ECC_GameTraceChannel1);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SphereCollision);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AProjectile::Move, MoveRate, true, MoveRate);
}


void AProjectile::Move()
{
	FVector movePosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(movePosition);
}

void AProjectile::SetDamageToActor(AActor* OtherActor)
{
	IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
	if (DamageTakerActor)
	{
		FDamageData damageData;
		damageData.DamageValue = Damage;
		damageData.Instigator = GetOwner();
		damageData.DamageMaker = this;
		DamageTakerActor->TakeDamage(damageData);
	}
}

void AProjectile::MakeImpulse(AActor* OtherActor)
{
	UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (mesh)
	{
		if (mesh->IsSimulatingPhysics())
		{
			FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
			forceVector.Normalize();
			mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
		}
	}
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Projectile overlap : %s"), *OtherActor->GetName());
	if (OtherActor)
	{
		SetDamageToActor(OtherActor);
		MakeImpulse(OtherActor);
	}
	Destroy();
}


void AProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;
	FQuat Rotation = FQuat::Identity;
	bool sweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		startPos,
		endPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		params
	);
	GetWorld()->DebugDrawTraceTag = "Explode Trace";
	if (sweepResult)
	{
		for (FHitResult hitResult : AttackHit)
		{
			AActor* otherActor = hitResult.GetActor();
			if (!otherActor)
				continue;
			SetDamageToActor(otherActor);
			MakeImpulse(otherActor);
		}
	}
}