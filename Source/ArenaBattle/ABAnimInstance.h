// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 *
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackMontage();
	void PlayArrowMontage();
	void PlayMagicMontage();
	void PlayChargingMontage();

	void JumpToAttackMontageSection(int32 NewSection);

	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	void SetDeadAnim() { IsDead = true; }
	UPROPERTY(EditAnyWhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);
private:
	UPROPERTY(EditAnyWhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	

	UPROPERTY(VisibleDefaultsOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ArrowMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MagicMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ChargingMontage;

	UPROPERTY(EditAnyWhere, BlueprintreadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	
};
