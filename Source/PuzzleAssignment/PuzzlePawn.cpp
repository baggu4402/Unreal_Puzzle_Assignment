#include "PuzzlePawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"

APuzzlePawn::APuzzlePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	CapsuleComponent->InitCapsuleSize(42.0f, 96.0f);
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionObjectType(ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetSimulatePhysics(false);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	MoveSpeed = 600.0f;
	AirControlRatio = 0.4f;

	YawSpeed = 120.0f;
	PitchSpeed = 80.0f;
	RollSpeed = 100.0f;

	bUseGravity = true;
	GravityAcceleration = -980.0f;
	GroundCheckDistance = 20.0f;

	MoveInput = FVector2D::ZeroVector;
	UpDownInput = 0.0f;
	RollInput = 0.0f;

	VerticalVelocity = 0.0f;
	bIsGrounded = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APuzzlePawn::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();
	CheckGround();
}

void APuzzlePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddDefaultMappingContext();
}

void APuzzlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckGround();
	ProcessMovement(DeltaTime);
	ApplyGravity(DeltaTime);
	CheckGround();
}

void APuzzlePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent == nullptr)
	{
		return;
	}

	if (MoveAction != nullptr)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APuzzlePawn::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APuzzlePawn::StopMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &APuzzlePawn::StopMove);
	}

	if (LookAction != nullptr)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APuzzlePawn::Look);
	}

	if (UpDownAction != nullptr)
	{
		EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &APuzzlePawn::UpDown);
		EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Completed, this, &APuzzlePawn::StopUpDown);
		EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Canceled, this, &APuzzlePawn::StopUpDown);
	}

	if (RollAction != nullptr)
	{
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APuzzlePawn::Roll);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Completed, this, &APuzzlePawn::StopRoll);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Canceled, this, &APuzzlePawn::StopRoll);
	}
}

void APuzzlePawn::Move(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();
}

void APuzzlePawn::StopMove(const FInputActionValue& Value)
{
	MoveInput = FVector2D::ZeroVector;
}

void APuzzlePawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();

	const UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	const float DeltaTime = World->GetDeltaSeconds();

	const float YawAmount = LookInput.X * YawSpeed * DeltaTime;
	
	const float PitchAmount = LookInput.Y * PitchSpeed * DeltaTime;

	AddActorLocalRotation(FRotator(PitchAmount, YawAmount, 0.0f));
}

void APuzzlePawn::UpDown(const FInputActionValue& Value)
{
	UpDownInput = Value.Get<float>();
}

void APuzzlePawn::StopUpDown(const FInputActionValue& Value)
{
	UpDownInput = 0.0f;
}

void APuzzlePawn::Roll(const FInputActionValue& Value)
{
	RollInput = Value.Get<float>();
}

void APuzzlePawn::StopRoll(const FInputActionValue& Value)
{
	RollInput = 0.0f;
}

void APuzzlePawn::ProcessMovement(float DeltaTime)
{
	const float CurrentMoveSpeed = bIsGrounded ? MoveSpeed : MoveSpeed * AirControlRatio;

	const FVector ForwardDirection = GetActorForwardVector();
	const FVector RightDirection = GetActorRightVector();
	const FVector UpDirection = GetActorUpVector();

	FVector MoveDirection =
		ForwardDirection * MoveInput.Y +
		RightDirection * MoveInput.X +
		UpDirection * UpDownInput;

	if (!MoveDirection.IsNearlyZero())
	{
		MoveDirection = MoveDirection.GetSafeNormal();

		const FVector MoveOffset = MoveDirection * CurrentMoveSpeed * DeltaTime;

		AddActorWorldOffset(MoveOffset, true);
	}

	if (!FMath::IsNearlyZero(RollInput))
	{
		const float RollAmount = RollInput * RollSpeed * DeltaTime;

		AddActorLocalRotation(FRotator(0.0f, 0.0f, RollAmount));
	}
}

void APuzzlePawn::ApplyGravity(float DeltaTime)
{
	if (!bUseGravity)
	{
		return;
	}

	if (!bIsGrounded)
	{
		VerticalVelocity += GravityAcceleration * DeltaTime;
	}
	else if (VerticalVelocity < 0.0f)
	{
		VerticalVelocity = 0.0f;
	}

	const FVector GravityOffset = FVector(0.0f, 0.0f, VerticalVelocity * DeltaTime);

	AddActorWorldOffset(GravityOffset, true);
}

void APuzzlePawn::CheckGround()
{
	if (GetWorld() == nullptr || CapsuleComponent == nullptr)
	{
		return;
	}
	
	if (UpDownInput > 0.1f)
	{
		bIsGrounded = false;
		VerticalVelocity = 0.0f;
		return;
	}

	const bool bWasGrounded = bIsGrounded;

	const FVector Start = GetActorLocation();
	const float CapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	const FVector End = Start - FVector(0.0f, 0.0f, CapsuleHalfHeight + GroundCheckDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bIsGrounded = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	if (bIsGrounded)
	{
		if (!bWasGrounded && VerticalVelocity < 0.0f)
		{
			VerticalVelocity = 0.0f;
		}

		if (VerticalVelocity <= 0.0f)
		{
			FVector NewLocation = GetActorLocation();
			NewLocation.Z = HitResult.ImpactPoint.Z + CapsuleHalfHeight;
			SetActorLocation(NewLocation);
		}
	}
}

void APuzzlePawn::AddDefaultMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController == nullptr)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

	if (LocalPlayer == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (Subsystem == nullptr)
	{
		return;
	}

	if (DefaultMappingContext != nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}