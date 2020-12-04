// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT(
		"/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ARROW_MONTAGE(TEXT(
		"/Game/Book/Animations/Arrow_montage.Arrow_Montage"));

	if (ARROW_MONTAGE.Succeeded())
	{
		ArrowMontage = ARROW_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Magic_MONTAGE(TEXT(
		"/Game/Book/Animations/Magic_Montage.Magic_Montage"));
	if (Magic_MONTAGE.Succeeded())
	{
		MagicMontage = Magic_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Charging_MONTAGE(TEXT(
		"/Game/Book/Animations/Charging_Montage.Charging_Montage"));
	if (Charging_MONTAGE.Succeeded())
	{
		ChargingMontage = Charging_MONTAGE.Object;
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);

}

void UABAnimInstance::PlayArrowMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(ArrowMontage, 1.0f);

}

void UABAnimInstance::PlayMagicMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(MagicMontage, 1.0f);
}

void UABAnimInstance::PlayChargingMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(ChargingMontage, 1.0f);
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}