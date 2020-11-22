#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "PublicCharacter.generated.h"

UCLASS()
class ARENABATTLE_API APublicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APublicCharacter();

	USkeletalMeshComponent* GetSpecificPawnMesh() const;

	FName GetWeaponAttachPoint() const;

	void EquipWeapon(class AABWeapon* Weapon);

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AArrow> ArrowClass;



protected:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponAttachPoint;
	UPROPERTY()
	class UABAnimInstance* ABAnim;

	TArray<class AABWeapon*> Inventory;

	class AABWeapon* CurrentWeapon;

	void AddWeapon(class AABWeapon* Weapon);

	void SetCurrentWeapon(class AABWeapon* NewWeapon, class AABWeapon* LastWeapon);

	void OnChangeWeapon();

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AABWeapon>> DefaultInventoryClasses;

	void SpawnDefaultInventory();

	void OnShoot();
};
