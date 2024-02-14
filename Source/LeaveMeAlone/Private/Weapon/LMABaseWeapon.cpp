// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/HitResult.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

void ALMABaseWeapon::Fire() {

	if (AutoFire) {
	
		GetWorldTimerManager().SetTimer(TimerAutoFire, this, &ALMABaseWeapon::Shoot, 60.0 / RateFire, true, 0.0f);
	}
	else{
	
		Shoot();
	}
}

void ALMABaseWeapon::OffFire() {

	if (GetWorldTimerManager().IsTimerActive(TimerAutoFire)) {

		GetWorldTimerManager().ClearTimer(TimerAutoFire);
	}
}

void ALMABaseWeapon::AutoFireOnOff() {

	AutoFire = !AutoFire;
}

void ALMABaseWeapon::ChangeClip() {

	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd) {

	const auto TraceFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceEffect, TraceStart);
	
	if (IsValid(TraceFX)) {

		TraceFX->SetNiagaraVariableVec3(TraceName, TraceEnd);
	}
}

void ALMABaseWeapon::Shoot() {

	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	FVector TracerEnd = TraceEnd;
	
	if (HitResult.bBlockingHit) {

		MakeDamage(HitResult);
		TracerEnd = HitResult.ImpactPoint;
	}

	SpawnTrace(TraceStart, TracerEnd);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);
	
	DecrementBullets();
}

void ALMABaseWeapon::DecrementBullets() {

	CurrentAmmoWeapon.Bullets--;

	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));

	if (IsCurrentClipEmpty()) {

		OffFire();
		OutBullets.Broadcast();
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const {

	return CurrentAmmoWeapon.Bullets == 0;
}

void ALMABaseWeapon::MakeDamage(const FHitResult& HitResult) {

	const auto Zombie = HitResult.GetActor();

	if (IsValid(Zombie)) {

		const auto Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		
		if (IsValid(Pawn)) { 

			const auto Controller = Pawn->GetController<APlayerController>();
			
			if (IsValid(Controller)) {

				Zombie->TakeDamage(Damage, FDamageEvent(), Controller, this);
			}
		}
	}
}

bool ALMABaseWeapon::IsCurrentClipComplete() const {

	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
}