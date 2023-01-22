// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class TANKOGEDDON_2023_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	ATankPawn();
	
	virtual void Tick(float DeltaTime) override;
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void RotateRight(float Value);

	void SetupCannon();
	void Fire();
	void FireSpecial();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPoint;

	UPROPERTY()
	ACannon* Cannon;
	
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
	float TurretInterpolationKey = 0.5f;

	

};
