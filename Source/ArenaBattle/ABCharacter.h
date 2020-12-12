
#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ChargingBall.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

	USkeletalMeshComponent* GetSpecificPawnMesh() const;

	FName GetWeaponAttachPoint() const;

	void EquipWeapon(class AABWeapon* Weapon);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO
	};

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;

	FVector DirectionToMove = FVector::ZeroVector;
	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = Camera)
	UCameraComponent* Camera;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Jump();

	void ViewChange();
	void Attack();
	void OnChangeWeapon();

	void OnShoot();
	void OnMagic();
	void MagicCharging();
	void SwordAttack();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;
	
	bool IsMoving;
	bool IsCharging;


	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintreadOnly, Category = Damage, Meta = (AllowPrivateAccess = true))
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AArrow> ArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMagicBall> MagicBallClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AChargingBall> ChargingBallClass;

	AChargingBall* Ball;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName WeaponAttachPoint;
	UPROPERTY()
		class UABAnimInstance* ABAnim;


	TArray<class AABWeapon*> Inventory;

	class AABWeapon* CurrentWeapon;

	void AddWeapon(class AABWeapon* Weapon);

	void SetCurrentWeapon(class AABWeapon* NewWeapon, class AABWeapon* LastWeapon);



	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AABWeapon>> DefaultInventoryClasses;

	void SpawnDefaultInventory();
};
