// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { 
		
		return HealthComponent; 
	}

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);
	
	// Maximum values camera separation.
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ArmLengthMax = 2800.0f;

	// Minimum values camera distance.
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ArmLengthMIn = 140.0f;

	// Speed of camera moving away and approaching. Proportional to distance.
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ArmLengthSpeed = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking")
	float MaxSprintTime = 10.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	FTimerHandle TimerSprintCharacterOff;

	float YRotation = -75.0f;
	float ArmLengthDefault = 1400.0f;
	float FOV = 55.0f;
	float SprintTime = 0.0f;
	float TimeForSprint = 0.0f;
	bool SprintCharacter = false;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveCameraForward();
	void MoveCameraBack();
	void OnDeath();
	void RotationPlayerOnCursor();
	void OnHealthChanged(float NewHealth);
	void SprintCharacterOn();
	void SprintCharacterOff();
};
