// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargingBall.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine.h"


// Sets default values
AChargingBall::AChargingBall()
{

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	RootComponent = Effect;
	Effect->SetupAttachment(RootComponent);
	ChargingBallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ChargingBallComp"));
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
