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

	YawSpeed = 120.0f;
	PitchSpeed = 80.0f;
	MinPitch = -60.0f;
	MaxPitch = 30.0f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APuzzlePawn::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();
}

void APuzzlePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddDefaultMappingContext();
}

void APuzzlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&APuzzlePawn::Move
		);
	}

	if (LookAction != nullptr)
	{
		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&APuzzlePawn::Look
		);
	}
}

void APuzzlePawn::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();

	const UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	const float DeltaTime = World->GetDeltaSeconds();

	const FVector LocalOffset = FVector(
		MoveValue.Y * MoveSpeed * DeltaTime,
		MoveValue.X * MoveSpeed * DeltaTime,
		0.0f
	);

	AddActorLocalOffset(LocalOffset, true);
}

void APuzzlePawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	const UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	const float DeltaTime = World->GetDeltaSeconds();

	const float YawAmount = LookValue.X * YawSpeed * DeltaTime;

	AddActorLocalRotation(FRotator(0.0f, YawAmount, 0.0f));

	const FRotator CurrentSpringArmRotation = SpringArmComponent->GetRelativeRotation();

	const float NewPitch = FMath::Clamp(
		CurrentSpringArmRotation.Pitch - LookValue.Y * PitchSpeed * DeltaTime,
		MinPitch,
		MaxPitch
	);

	SpringArmComponent->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
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