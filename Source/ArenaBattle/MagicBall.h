// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "BombBall.h"
#include "GameFramework/Actor.h"
#include "MagicBall.generated.h"


UCLASS()
class ARENABATTLE_API AMagicBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagicBall();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleDefaultsOnly, Category = Box)
		class UBoxComponent* MagicBallCollision;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
		USkeletalMeshComponent* MagicBallMesh;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABombBall> BombBallClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* MagicBallMovement;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;


	/*UFUNCTION()
		void BombOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);*/


};
