// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "BombBall.generated.h"

UCLASS()
class ARENABATTLE_API ABombBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABombBall();

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* BombBallMovement;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;

	void OnEffectFinished(class UParticleSystemComponent* PSystem);
};
