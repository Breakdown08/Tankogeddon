#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Unit.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class TANKOGEDDON_2023_API ATankPawn : public AUnit
{
	GENERATED_BODY()

public:
	ATankPawn();
	
	virtual void Tick(float DeltaTime) override;
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void RotateRight(float Value);
	
	void SwapCannons();
	void SetupCannon(TSubclassOf<ACannon> newCannon);
	void FireSpecial();
	TArray<FVector> GetPatrollingPoints();
	void SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> SecondCannonClass;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", META = (MakeEditWidget = true))
	TArray<ATargetPoint*> PatrollingPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

private:
	class ATankController* TankController;
	
	float ForwardMoveAxisValue = 0.0f;
	float RightMoveAxisValue = 0.0f;
	float RotateRightAxisValue = 0.0f;
	float CurrentRightAxisValue = 0.0f;
	float RotateInterpolationKey = 0.1f;
};

