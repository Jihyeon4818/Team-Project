// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PublicCharacter.h"
#include "Engine.h"


// Sets default values
AArrow::AArrow()
{
	ArrowCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Arrow Comp"));
	ArrowCollision->InitSphereRadius(20.0f);
	ArrowCollision->BodyInstance.SetCollisionProfileName("Arrow");
	//ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);
	RootComponent = ArrowCollision;

	ArrowMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowComp"));
	ArrowMovement->UpdatedComponent = ArrowCollision;
	ArrowMovement->InitialSpeed = 3000.0f;
	ArrowMovement->MaxSpeed = 3000.0f;
	ArrowMovement->bRotationFollowsVelocity = false;
	InitialLifeSpan = 3.0f;
}

void AArrow::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		Destroy();
		if (OtherActor->IsA(APublicCharacter::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Collision");
			UGameplayStatics::ApplyPointDamage(SweepResult.Actor.Get(), 1, -SweepResult.ImpactNormal, SweepResult, NULL, this, UDamageType::StaticClass());
		}
	}
}

