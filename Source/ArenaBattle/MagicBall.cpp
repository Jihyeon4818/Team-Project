// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicBall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PublicCharacter.h"
#include "Engine.h"


// Sets default values
AMagicBall::AMagicBall()
{
	MagicBallCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MagicBall Comp"));

	MagicBallCollision->OnComponentBeginOverlap.AddDynamic(this, &AMagicBall::OnComponentBeginOverlap);
	RootComponent = MagicBallCollision;
	MagicBallMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagicBallMesh"));
	MagicBallMesh->SetupAttachment(RootComponent);

	MagicBallCollision->SetBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	MagicBallCollision->SetCollisionProfileName("MagicBall");
	

	MagicBallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MagicBallComp"));
	MagicBallMovement->UpdatedComponent = MagicBallCollision;
	MagicBallMovement->InitialSpeed = 1000.0f;
	MagicBallMovement->MaxSpeed = 1000.0f;
	MagicBallMovement->ProjectileGravityScale = 0.0f;
	MagicBallMovement->bRotationFollowsVelocity = false;

}

void AMagicBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetLifeSpan(1.0f);
}

void AMagicBall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UGameplayStatics::ApplyPointDamage(SweepResult.Actor.Get(), 200, -SweepResult.ImpactNormal, SweepResult, NULL, this, UDamageType::StaticClass());
		Destroy();
	}
}
