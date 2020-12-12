// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ABCharacter.h"
#include "Engine.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AArrow::AArrow()
{

	ArrowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Arrow Comp"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	
	ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnComponentBeginOverlap);
	RootComponent = ArrowCollision;
	ArrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	ArrowCollision->SetBoxExtent(FVector(35.0f,5.0f,5.0f));
	ArrowCollision->SetCollisionProfileName("Arrow");
	
	ArrowMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowComp"));
	ArrowMovement->UpdatedComponent = ArrowCollision;
	ArrowMovement->InitialSpeed = 3000.0f;
	ArrowMovement->MaxSpeed = 3000.0f;
	ArrowMovement->ProjectileGravityScale = 0.0f;
	ArrowMovement->bRotationFollowsVelocity = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_LAVADRIPS(TEXT
	("/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_LavaDrips.P_LavaDrips"));
	if (P_LAVADRIPS.Succeeded())
	{
		Effect->SetTemplate(P_LAVADRIPS.Object);
		//Effect->bAutoActivate = true;

	}
}

void AArrow::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetLifeSpan(1.5f);
	Effect->Activate(true);
}



void AArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (!OtherActor->IsA(AABCharacter::StaticClass()))
		{
			UGameplayStatics::ApplyPointDamage(SweepResult.Actor.Get(), 200, -SweepResult.ImpactNormal, SweepResult, NULL, this, UDamageType::StaticClass());
			Destroy();
		}
	}
}
