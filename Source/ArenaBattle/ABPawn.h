// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ABPawn.generated.h"

UCLASS()
class ARENABATTLE_API AABPawn : public APawn
{
	GENERATED_BODY()

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);

public:
	// Sets default values for this pawn's properties
	AABPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnyWhere, Category = Collision)
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnyWhere, Category = Visual)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnyWhere, Category = MoveMent)
	UFloatingPawnMovement* Movement;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
	UCameraComponent* Camera;
};
