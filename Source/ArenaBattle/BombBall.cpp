// Fill out your copyright notice in the Description page of Project Settings.


#include "BombBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine.h"

// Sets default values
ABombBall::ABombBall()
{

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	RootComponent = Effect;
	Effect->SetupAttachment(RootComponent);
	BombBallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BombBallComp"));
	BombBallMovement->ProjectileGravityScale = 0.0f;
	BombBallMovement->bRotationFollowsVelocity = false;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_BOMB(TEXT
	("/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_Fire_AOE_Blast_Spin_mobile.P_Fire_AOE_Blast_Spin_mobile"));
	if (P_BOMB.Succeeded())
	{
		Effect->SetTemplate(P_BOMB.Object);
	}
	Effect->OnSystemFinished.AddDynamic(this, &ABombBall::OnEffectFinished);
}

void ABombBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Effect->Activate(true);
}

void ABombBall::OnEffectFinished(class UParticleSystemComponent* PSystem)
{
	Destroy();
}

