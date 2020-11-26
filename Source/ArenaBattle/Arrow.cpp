// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PublicCharacter.h"
#include "Engine.h"


// Sets default values
AArrow::AArrow()
{
	ArrowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Arrow Comp"));
	
	
	//ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);
	ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnComponentBeginOverlap);
	RootComponent = ArrowCollision;
	ArrowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(RootComponent);

	ArrowCollision->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_WEAPON(
		TEXT("/Game/Weapon/Blade_BohntaPick/SK_Blade_BohntaPick.SK_Blade_BohntaPick"));
	if (SK_WEAPON.Succeeded())
	{
		ArrowMesh->SetSkeletalMesh(SK_WEAPON.Object);
	}
	ArrowCollision->SetCollisionProfileName("Arrow");
	
	
	ArrowMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowComp"));
	ArrowMovement->UpdatedComponent = ArrowCollision;
	ArrowMovement->InitialSpeed = 3000.0f;
	ArrowMovement->MaxSpeed = 3000.0f;
	ArrowMovement->bRotationFollowsVelocity = false;
	
}

void AArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABLOG_S(Warning);
	Destroy();
	UGameplayStatics::ApplyPointDamage(SweepResult.Actor.Get(), 200, -SweepResult.ImpactNormal, SweepResult, NULL, this, UDamageType::StaticClass());
}
