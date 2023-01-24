#include "TankPawn.h"
#include "TankController.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATankPawn::ATankPawn() 
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY TANK PAWN"));
	Super::BeginPlay();
	TankController = Cast<ATankController>(GetController());
	SetupCannon(CannonClass);
}


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
	
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, RotateRightAxisValue, RotateInterpolationKey);
	
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

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannon)
{
	UE_LOG(LogTemp, Warning, TEXT("SETUP CANNON TANK PAWN"));
	if (!newCannon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR SETUP CANNON"));
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("TRY SETUP"));
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(newCannon, params);
	CannonClass = newCannon;
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}



void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}


void ATankPawn::SwapCannons()
{
	if (Cannon)
	{
		Cannon->Destroy();
		FActorSpawnParameters params;
		params.Instigator = this;
		params.Owner = this;
		TSubclassOf<ACannon> tempCannon = SecondCannonClass;
		SecondCannonClass = CannonClass;
		CannonClass = tempCannon;
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}


