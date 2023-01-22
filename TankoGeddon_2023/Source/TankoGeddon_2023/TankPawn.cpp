// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "TankController.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TankController = Cast<ATankController>(GetController());
	SetupCannon();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector movePosition;
	if (RightMoveAxisValue != 0)
	{
		movePosition = currentLocation + RightVector * MovementSpeed * RightMoveAxisValue * DeltaTime;
	}
	else
	{
		movePosition = currentLocation + ForwardVector * MovementSpeed * ForwardMoveAxisValue * DeltaTime;
	}
	SetActorLocation(movePosition);

	//UE_LOG(LogTemp, Warning, TEXT("RotateRightAxisValue %f"), RotateRightAxisValue);
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, RotateRightAxisValue, RotateInterpolationKey);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentRightAxisValue %f RotateRightAxisValue %f"), CurrentRightAxisValue, RotateRightAxisValue);
	
	float yawRotation = CurrentRightAxisValue * RotationSpeed * DeltaTime;
	FRotator currentRotation = GetActorRotation();

	yawRotation += currentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotation);

	if (TankController)
	{
		FVector MousePos = TankController->GetMousePos();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
		FRotator turretRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = turretRotation.Pitch;
		targetRotation.Roll = turretRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(targetRotation, turretRotation, TurretInterpolationKey));
	}
}

void ATankPawn::MoveForward(float Value)
{
	ForwardMoveAxisValue = Value;
}


void ATankPawn::MoveRight(float Value)
{
	RightMoveAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	RotateRightAxisValue = Value;
}

void ATankPawn::SetupCannon()
{
	if (!CannonClass)
	{
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}


void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}
