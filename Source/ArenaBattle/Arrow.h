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

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName HitName;

	UPROPERTY(VisibleDefaultsOnly, Category = Sphere)
	class USphereComponent* ArrowCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ArrowMovement;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE class USphereComponent* GetArrowCollision() const { return ArrowCollision; }
	FORCEINLINE class UProjectileMovementComponent* GetArrowMovement() const { return ArrowMovement; }

};
