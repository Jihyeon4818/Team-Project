#include "ABCharacter.h"
#include "MagicBall.h"
#include "DrawDebugHelpers.h"
#include "ABWeapon.h"
#include "Arrow.h"
#include "ABAnimInstance.h"
#include "Engine.h"

// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 00.0f, 0.0f));

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT
	("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));

	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT
	("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));

	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}*/
	
	SetControlMode(EControlMode::GTA);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// 점프 하는 값을 800으로 설정
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	IsAttacking = false;
	IsMoving = true;
	IsCharging = false;

	MaxCombo = 4;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	Health = 10000.0f;
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (NewControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		//GetController()->SetControlRotation(GetActorRotation());
		ArmLengthTo = 450.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;//캐릭터가 벽에 가까워지면 줌인
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}

}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmRotationSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:

		SpringArm->RelativeRotation = FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}

		break;
	}
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("ChangeWeapon"), EInputEvent::IE_Pressed, this, &AABCharacter::OnChangeWeapon);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);
	ABAnim->OnNextAttackCheck.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});

	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);
}

void AABCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(50.f),
		Params);
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Acttor Name: %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(10.0f, DamageEvent, GetController(), this);
		}
	}
}


float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor: %s took Damage : %f"), *GetName(), FinalDamage);

	Health -= FinalDamage;

	if (Health <= 0.0f)
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}
	return FinalDamage;
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	IsAttacking = false;
	AttackEndComboState();
}



void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::UpDown(float NewAxisValue)
{
	if (IsMoving== true)
	{
		//ABLOG(Warning, TEXT("%f"), NewAxisValue);
		switch (CurrentControlMode)
		{
		case EControlMode::GTA:
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
			break;
		case EControlMode::DIABLO:
			DirectionToMove.X = NewAxisValue;
			break;
		}
	}
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	if (IsMoving == true)
	{
		//ABLOG(Warning, TEXT("%f"), NewAxisValue);
		switch (CurrentControlMode)
		{
		case EControlMode::GTA:
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
			break;
		case EControlMode::DIABLO:
			DirectionToMove.Y = NewAxisValue;
			break;
		}
	}
}

void AABCharacter::LookUp(float NewAxisValue)
{

		//ABLOG(Warning, TEXT("%f"), NewAxisValue);
		switch (CurrentControlMode)
		{
		case EControlMode::GTA:
			AddControllerPitchInput(NewAxisValue);
			break;
		}
	

}

void AABCharacter::Turn(float NewAxisValue)
{
	
		//ABLOG(Warning, TEXT("%f"), NewAxisValue);
		switch (CurrentControlMode)
		{
		case EControlMode::GTA:
			AddControllerYawInput(NewAxisValue);
			break;
		}
	

}

void AABCharacter::Jump()
{
	if (IsMoving == true)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void AABCharacter::Attack()
{
	if (CurrentWeapon == Inventory[1])
	{
		SwordAttack();
	}

	else if (CurrentWeapon == Inventory[2])
	{
		OnShoot();
	}

	else if (CurrentWeapon)
	{
		if (IsCharging)
		{
			OnMagic();
		}
		else
		{
			MagicCharging();
		}
	}
}

void AABCharacter::SwordAttack()
{
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}

	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AABCharacter::OnShoot()
{
	if (!IsAttacking)
	{
		ABCHECK(CurrentCombo == 0);
		IsAttacking = true;
		if (ArrowClass != NULL)
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(100.0f, 30.0f, 10.0f));

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				ABAnim->PlayArrowMontage();
				World->SpawnActor<AArrow>(ArrowClass, SpawnLocation, SpawnRotation);
			}
		}
	}

}

void AABCharacter::MagicCharging()
{
	if (!IsAttacking&&!GetMovementComponent()->IsFalling())
	{
		
		ABCHECK(CurrentCombo == 0);
		IsAttacking = true;
		if (ArrowClass != NULL)
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(80.0f, 30.0f, 10.0f));

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				ABAnim->PlayChargingMontage();
				Ball = World->SpawnActor<AChargingBall>(ChargingBallClass, SpawnLocation, SpawnRotation);
				IsMoving = false;
				IsCharging = true;
			}
		}
	}
}

void AABCharacter::OnMagic()
{
	if (!IsAttacking)
	{
		ABCHECK(CurrentCombo == 0);
		IsAttacking = true;
		if (ArrowClass != NULL)
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(80.0f, 30.0f, 10.0f));

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				Ball->Destroy();
				ABAnim->PlayMagicMontage();
				World->SpawnActor<AMagicBall>(MagicBallClass, SpawnLocation, SpawnRotation);
				IsCharging = false;
				IsMoving = true;

			}
		}
	}

}

void AABCharacter::OnChangeWeapon()
{
	if (!IsAttacking)
	{
		const int32 CurrentWeaponIndex = Inventory.IndexOfByKey(CurrentWeapon);

		AABWeapon* NextWeapon = Inventory[(CurrentWeaponIndex + 1) % Inventory.Num()];

		EquipWeapon(NextWeapon);
	}
}

USkeletalMeshComponent* AABCharacter::GetSpecificPawnMesh() const
{
	return GetMesh();
}

FName AABCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

void AABCharacter::AddWeapon(AABWeapon* Weapon)
{
	if (Weapon)
	{
		Inventory.AddUnique(Weapon);
	}
}

void AABCharacter::SetCurrentWeapon(class AABWeapon* NewWeapon, class AABWeapon* LastWeapon)
{
	AABWeapon* LocalLastWeapon = NULL;

	if (LastWeapon != NULL)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);
		NewWeapon->OnEquip(LastWeapon);
	}
}

void AABCharacter::EquipWeapon(AABWeapon* Weapon)
{
	if (Weapon)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
}

void AABCharacter::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = DefaultInventoryClasses.Num();

	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo;

			AABWeapon* NewWeapon = GetWorld()->SpawnActor<AABWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			NewWeapon->WeaponMesh->SetHiddenInGame(true);
			AddWeapon(NewWeapon);
		}
	}

	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}

