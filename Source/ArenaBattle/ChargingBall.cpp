// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargingBall.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine.h"


// Sets default values
AChargingBall::AChargingBall()
{
	ChargingBallCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ChargingBall Comp"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = ChargingBallCollision;
	ChargingBallMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChargingBallMesh"));
	ChargingBallMesh->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	ChargingBallCollision->SetBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	ChargingBallCollision->SetCollisionProfileName("MagicBall");


	ChargingBallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ChargingBallComp"));
	ChargingBallMovement->UpdatedComponent = ChargingBallCollision;
	ChargingBallMovement->ProjectileGravityScale = 0.0f;
	ChargingBallMovement->bRotationFollowsVelocity = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_LAVADRIPS(TEXT
	("/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_LavaDrips.P_LavaDrips"));
	if (P_LAVADRIPS.Succeeded())
	{
		Effect->SetTemplate(P_LAVADRIPS.Object);
	}
	
}

void AChargingBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Effect->Activate(true);
}
