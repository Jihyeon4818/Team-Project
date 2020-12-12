#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABCharacter.h"
#include "Engine.h"
#include "ABWeapon.generated.h"


UCLASS()
class ARENABATTLE_API AABWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AABWeapon(const class FObjectInitializer& ObjectInitializer);

	void SetOwningPawn(AABCharacter* NewOwner);

	void AttachMeshToPawn();

	void OnEquip(const AABWeapon* LastWeapon);
	void OnUnEquip();
public:
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		USkeletalMeshComponent* WeaponMesh;

protected:
	class AABCharacter* MyPawn;

};