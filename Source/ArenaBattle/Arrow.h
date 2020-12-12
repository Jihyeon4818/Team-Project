// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"


UCLASS()
class ARENABATTLE_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

	virtual void PostInitializeComponents() override;


	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	class UBoxComponent* ArrowCollision;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	USkeletalMeshComponent* ArrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ArrowMovement;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

};
