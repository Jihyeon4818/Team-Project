// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargingBall.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine.h"


// Sets default values
AChargingBall::AChargingBall()
{
	ChargingBallCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ChargingBall Comp"));

	RootComponent = ChargingBallCollision;
	ChargingBallMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChargingBallMesh"));
	ChargingBallMesh->SetupAttachment(RootComponent);

	ChargingBallCollision->SetBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	ChargingBallCollision->SetCollisionProfileName("MagicBall");


	ChargingBallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ChargingBallComp"));
	ChargingBallMovement->UpdatedComponent = ChargingBallCollision;
	ChargingBallMovement->ProjectileGravityScale = 0.0f;
	ChargingBallMovement->bRotationFollowsVelocity = false;

}


