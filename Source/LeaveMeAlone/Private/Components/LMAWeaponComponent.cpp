// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const {

	if (IsValid(Weapon)) {

		AmmoWeapon = Weapon->GetCurrentAmmoWeapon();

		return true;
	}

	return false;
}

void ULMAWeaponComponent::Fire() {

	if (IsValid(Weapon) && !AnimReloading && !BlockingWeapons) {

		Weapon->Fire();
	}
}

void ULMAWeaponComponent::OffFire() {

	Weapon->OffFire();
}

void ULMAWeaponComponent::AutoFireOnOff() {

	Weapon->AutoFireOnOff();
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	SpawnWeapon();
	InitAnimNotify();
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULMAWeaponComponent::SpawnWeapon() {

	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	Weapon->OutBullets.AddUObject(this, &ULMAWeaponComponent::OutBullets);

	if (IsValid(Weapon)) {

		const auto Character = Cast<ACharacter>(GetOwner());

		if (Character) {

			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
		}
	}
}

void ULMAWeaponComponent::Reload() {

	if (!BlockingWeapons) {
	
		Recharge();
	}
}

void ULMAWeaponComponent::OnBlockingWeapons() {

	BlockingWeapons = true;
	OffFire();
}

void ULMAWeaponComponent::OffBlockingWeapons() {

	BlockingWeapons = false;
}

void ULMAWeaponComponent::InitAnimNotify() {

	if (IsValid(ReloadMontage)) {

		const auto NotifiesEvents = ReloadMontage->Notifies;
		
		for (auto NotifyEvent : NotifiesEvents) {

			auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
			
			if (ReloadFinish) {

				ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
				
				break;
			}
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh) {

	const auto Character = Cast<ACharacter>(GetOwner());
	
	if (Character->GetMesh() == SkeletalMesh) {

		AnimReloading = false;
		Weapon->ChangeClip();
	}
}

bool ULMAWeaponComponent::CanReload() const {

	return !AnimReloading && !Weapon->IsCurrentClipComplete();
}

void ULMAWeaponComponent::OutBullets() {

	Recharge();
}

void ULMAWeaponComponent::Recharge() {

    if (CanReload()) {

		Weapon->OffFire();
		AnimReloading = true;
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		Character->PlayAnimMontage(ReloadMontage);
	}
}

