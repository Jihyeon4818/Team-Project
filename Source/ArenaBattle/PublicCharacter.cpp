#include "PublicCharacter.h"
#include "ABWeapon.h"
#include "Arrow.h"
#include "ABAnimInstance.h"
#include "Engine.h"

// Sets default values
APublicCharacter::APublicCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

USkeletalMeshComponent* APublicCharacter::GetSpecificPawnMesh() const
{
	return GetMesh();
}

FName APublicCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

void APublicCharacter::AddWeapon(AABWeapon* Weapon)
{
	if (Weapon)
	{
		Inventory.AddUnique(Weapon);
	}
}

void APublicCharacter::SetCurrentWeapon(class AABWeapon* NewWeapon, class AABWeapon* LastWeapon)
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

void APublicCharacter::EquipWeapon(AABWeapon* Weapon)
{
	if (Weapon)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
}

void APublicCharacter::SpawnDefaultInventory()
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


