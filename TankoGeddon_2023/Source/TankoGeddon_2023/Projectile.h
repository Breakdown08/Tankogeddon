// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDDON_2023_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	virtual void Start();
	void Explode();

	FTimerHandle MoveTimer;

protected:
	UFUNCTION()
	virtual void Move();

	UFUNCTION()
	void SetDamageToActor(AActor* OtherActor);
	
	UFUNCTION()
	void MakeImpulse(AActor* OtherActor);
	
	
	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	float MoveRate = 0.005f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	float Damage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float PushForce = 1000;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	float ExplodeRadius = 200;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	bool IsCanExplode = true;

};