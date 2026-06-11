#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PuzzlePawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PUZZLEASSIGNMENT_API APuzzlePawn : public APawn
{
	GENERATED_BODY()

public:
	APuzzlePawn();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void UpDown(const FInputActionValue& Value);
	void StopUpDown(const FInputActionValue& Value);

	void Roll(const FInputActionValue& Value);
	void StopRoll(const FInputActionValue& Value);

	void AddDefaultMappingContext();

	void ProcessMovement(float DeltaTime);
	void ApplyGravity(float DeltaTime);
	void CheckGround();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* UpDownAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AirControlRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look", meta = (AllowPrivateAccess = "true"))
	float YawSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look", meta = (AllowPrivateAccess = "true"))
	float PitchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look", meta = (AllowPrivateAccess = "true"))
	float RollSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
	bool bUseGravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
	float GravityAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
	float GroundCheckDistance;

	FVector2D MoveInput;
	float UpDownInput;
	float RollInput;

	float VerticalVelocity;
	bool bIsGrounded;
};