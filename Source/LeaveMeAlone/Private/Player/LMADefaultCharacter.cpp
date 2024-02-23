// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Player/LMADefaultCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"


// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLengthDefault;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SprintTime = MaxSprintTime;
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CursorMaterial) {

		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead())) {

		RotationPlayerOnCursor();
	}

	TimeForSprint += DeltaTime;
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveCameraForward", IE_Pressed, this, &ALMADefaultCharacter::MoveCameraForward);
	PlayerInputComponent->BindAction("MoveCameraBack", IE_Pressed, this, &ALMADefaultCharacter::MoveCameraBack);
	PlayerInputComponent->BindAction("AcceleratedCharacterMovement", IE_Pressed, this, &ALMADefaultCharacter::SprintCharacterOn);
	PlayerInputComponent->BindAction("AcceleratedCharacterMovement", IE_Released, this, &ALMADefaultCharacter::SprintCharacterOff);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::OffFire);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::OffFire);
	PlayerInputComponent->BindAction("AutoFireOnOff", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::AutoFireOnOff);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
}

void ALMADefaultCharacter::MoveForward(float Value) {

	if (!SprintCharacter) {

		Value /= 2;
	}

	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value) {

	if (!SprintCharacter) {

		Value /= 2;
	}

	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::MoveCameraForward() {

	if (SpringArmComponent->TargetArmLength > ArmLengthMIn) {

		SpringArmComponent->TargetArmLength -= SpringArmComponent->TargetArmLength / ArmLengthSpeed;
	}
}

void ALMADefaultCharacter::MoveCameraBack() {

	if (SpringArmComponent->TargetArmLength < ArmLengthMax)	{

		SpringArmComponent->TargetArmLength += SpringArmComponent->TargetArmLength / ArmLengthSpeed;
	}
}

void ALMADefaultCharacter::SprintCharacterOn() {

	SprintTime = FMath::Clamp(SprintTime + TimeForSprint, 0.0f, MaxSprintTime);

	if (SprintTime > MaxSprintTime * 0.1) {

		SprintCharacter = true;

		WeaponComponent->OnBlockingWeapons();
		GetWorldTimerManager().SetTimer(TimerSprintCharacterOff, this, &ALMADefaultCharacter::SprintCharacterOff, SprintTime);
	}
}

void ALMADefaultCharacter::SprintCharacterOff() {

	SprintCharacter = false;

	WeaponComponent->OffBlockingWeapons();
	
	if (GetWorldTimerManager().IsTimerActive(TimerSprintCharacterOff)) {

		TimeForSprint = 0.0f;
		
		SprintTime = GetWorldTimerManager().GetTimerRemaining(TimerSprintCharacterOff);
		GetWorldTimerManager().ClearTimer(TimerSprintCharacterOff);
	}

	if (SprintTime < 0) {

		SprintTime = 0.0f;
	}
}

void ALMADefaultCharacter::OnDeath() {

	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (IsValid(Controller)) {

		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::RotationPlayerOnCursor() {

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (IsValid(PC)) {

		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		
		if (IsValid(Controller)) {

			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}