// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;

DECLARE_MULTICAST_DELEGATE(FOutBullets)

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();

	FOutBullets OutBullets;

	void Fire();
	void OffFire();
	void AutoFireOnOff();
	void ChangeClip();
	bool IsCurrentClipComplete() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{30, 0, true};

	// Rate of fire - rds/min.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float RateFire = 600.0f;

	// Automatic weapon firing: true - enable , false - disable.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool AutoFire = true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Shoot();
	void DecrementBullets();
	bool IsCurrentClipEmpty() const;
	

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FAmmoWeapon CurrentAmmoWeapon;
	FTimerHandle TimerAutoFire;
};
