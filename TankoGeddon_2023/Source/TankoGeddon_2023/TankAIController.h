// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATankPawn;

UCLASS()
class TANKOGEDDON_2023_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ATankPawn* TankPawn;

	UPROPERTY()
	APawn* PlayerPawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", META = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;

	int32 CurrentPatrolPointIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MovementAccurency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;

	bool IsPlayerSeen();
	

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	float GetRotationValue();

	void Targeting();

	void RotateToPlayer();

	bool IsPlayerInRange();
	bool CanFire();

	void Fire();

	void Initialize();
};
