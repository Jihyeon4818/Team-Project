// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PublicCharacter.h"
#include "Engine.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AArrow::AArrow()
{
	ArrowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Arrow Comp"));
	
	ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnComponentBeginOverlap);
	RootComponent = ArrowCollision;
	ArrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(RootComponent);

	ArrowCollision->SetBoxExtent(FVector(35.0f,5.0f,5.0f));
	ArrowCollision->SetCollisionProfileName("Arrow");
	
	ArrowMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowComp"));
	ArrowMovement->UpdatedComponent = ArrowCollision;
	ArrowMovement->InitialSpeed = 3000.0f;
	ArrowMovement->MaxSpeed = 3000.0f;
	ArrowMovement->ProjectileGravityScale = 0.0f;
	ArrowMovement->bRotationFollowsVelocity = false;
	
}

void AArrow::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetLifeSpan(1.0f);
}

void AArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UGameplayStatics::ApplyPointDamage(SweepResult.Actor.Get(), 200, -SweepResult.ImpactNormal, SweepResult, NULL, this, UDamageType::StaticClass());
		Destroy();
	}
}
