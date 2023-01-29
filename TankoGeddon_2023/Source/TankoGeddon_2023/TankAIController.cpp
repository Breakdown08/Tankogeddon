#include "TankAIController.h"
#include "TankPawn.h"
#include "Kismet/KismetArrayLibrary.h"
#include "DrawDebugHelpers.h"

bool ATankAIController::IsPlayerSeen()
{
	return TankPawn->IsPlayerSeen(this->TankPawn, Cast<AUnit>(PlayerPawn));
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetMovementAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPoints();

	for (FVector point : points)
	{
		PatrollingPoints.Add(point + pawnLocation);
	}
	CurrentPatrolPointIndex = 0;
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TankPawn->MoveForward(1);
	float rotationValue = GetRotationValue();
	TankPawn->RotateRight(rotationValue);
	Targeting();
}

float ATankAIController::GetRotationValue()
{
	FVector currentPoint = PatrollingPoints[CurrentPatrolPointIndex];
	FVector pawnLocation = TankPawn->GetActorLocation();

	if (FVector::Distance(currentPoint, pawnLocation) <= MovementAccurency)
	{
		CurrentPatrolPointIndex++;
		if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
		{
			CurrentPatrolPointIndex = 0;
		}
	}
	FVector moveDirection = currentPoint - pawnLocation;
	moveDirection.Normalize();

	FVector forwardDirection = TankPawn->GetActorForwardVector();
	FVector rightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), pawnLocation, currentPoint, FColor::Green, false, 0.1f, 0, 5);

	float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
	float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));

	float rotaionValue = 0;
	if (forwardAngle > 5)
	{
		rotaionValue = 1;
	}
	if (rightAngle > 90)
	{
		rotaionValue = -rotaionValue;
	}
	//TankPawn->RotateRight(rotaionValue);
	return rotaionValue;
}

void ATankAIController::Targeting()
{
	if (CanFire())
	{
		Fire();
	}
	else
	{
		RotateToPlayer();
	}
}

void ATankAIController::RotateToPlayer()
{
	if (IsPlayerInRange())
	{
		TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
	}
}

bool ATankAIController::IsPlayerInRange()
{
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool ATankAIController::CanFire()
{
	if (!IsPlayerSeen())
	{
		return false;
	}
	FVector targetingDir = TankPawn->GetTurretForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
	return aimAngle <= Accurency;
}

void ATankAIController::Fire()
{
	TankPawn->Fire();
}

