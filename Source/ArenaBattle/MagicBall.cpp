// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicBall.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine.h"


// Sets default values
AMagicBall::AMagicBall()
{

	PrimaryActorTick.bCanEverTick = true;
	MagicBallCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MagicBall Comp"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	
	//MagicBallCollision->OnComponentBeginOverlap.AddDynamic(this, &AMagicBall::BombOverlap);
	RootComponent = MagicBallCollision;
	MagicBallMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagicBallMesh"));
	MagicBallMesh->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	MagicBallCollision->SetBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	MagicBallCollision->SetCollisionProfileName("MagicBall");


	MagicBallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MagicBallComp"));
	MagicBallMovement->UpdatedComponent = MagicBallCollision;
	MagicBallMovement->InitialSpeed = 1000.0f;
	MagicBallMovement->MaxSpeed = 1000.0f;
	MagicBallMovement->ProjectileGravityScale = 0.0f;
	MagicBallMovement->bRotationFollowsVelocity = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_LAVADRIPS(TEXT
	("/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_LavaDrips.P_LavaDrips"));
	if (P_LAVADRIPS.Succeeded())
	{
		Effect->SetTemplate(P_LAVADRIPS.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BOMB(TEXT
	("/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_Fire_AOE_Blast_Spin_mobile.P_Fire_AOE_Blast_Spin_mobile"));
}



void AMagicBall::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetLifeSpan(1.5f);
	Effect->Activate(true);
}

void AMagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLifeSpan() < 0.5f)
	{
		FVector BombLocation = MagicBallCollision->GetComponentLocation();
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, BombLocation, 600.0f, nullptr, TArray<AActor*>(), this, false, ECC_Visibility);
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation();
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			World->SpawnActor<ABombBall>(BombBallClass, SpawnLocation, SpawnRotation);
		};
		Destroy();
	}
}


/*void AMagicBall::BombOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FVector BombLocation = MagicBallCollision->GetComponentLocation();
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, BombLocation, 400.0f, nullptr, TArray<AActor*>(), this, false,ECC_Visibility);
	Destroy();
}*/


