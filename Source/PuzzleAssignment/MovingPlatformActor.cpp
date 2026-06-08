#include "MovingPlatformActor.h"

AMovingPlatformActor::AMovingPlatformActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	MoveSpeed = 200.0f;
	MaxRange = 500.0f;
	MoveDirection = FVector(1.0f, 0.0f, 0.0f);

	bUseRandomTimer = true;
	RandomizeInterval = 20.0f;
	RandomPositionRange = 600.0f;

	CurrentDistance = 0.0f;
	MoveDirectionSign = 1;
}

void AMovingPlatformActor::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
	StartLocation = OriginalLocation;
	MoveDirection = MoveDirection.GetSafeNormal();

	if (bUseRandomTimer)
	{
		GetWorld()->GetTimerManager().SetTimer(
			RandomMoveTimerHandle,
			this,
			&AMovingPlatformActor::RandomizeMovement,
			RandomizeInterval,
			true
		);
	}
}

void AMovingPlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float MoveAmount = MoveSpeed * DeltaTime * MoveDirectionSign;

	CurrentDistance += MoveAmount;

	if (FMath::Abs(CurrentDistance) >= MaxRange)
	{
		MoveDirectionSign *= -1;
		CurrentDistance = FMath::Clamp(CurrentDistance, -MaxRange, MaxRange);
	}

	const FVector NewLocation = StartLocation + MoveDirection * CurrentDistance;

	SetActorLocation(NewLocation);
}

void AMovingPlatformActor::SetMoveSettings(float NewMoveSpeed, float NewMaxRange, const FVector& NewMoveDirection)
{
	MoveSpeed = NewMoveSpeed;
	MaxRange = NewMaxRange;
	MoveDirection = NewMoveDirection.GetSafeNormal();

	StartLocation = GetActorLocation();
	CurrentDistance = 0.0f;
	MoveDirectionSign = 1;
}

void AMovingPlatformActor::RandomizeMovement()
{
	const float RandomX = FMath::RandRange(-RandomPositionRange, RandomPositionRange);
	const float RandomY = FMath::RandRange(-RandomPositionRange, RandomPositionRange);

	StartLocation = OriginalLocation + FVector(RandomX, RandomY, 0.0f);
	SetActorLocation(StartLocation);

	MoveSpeed = FMath::RandRange(100.0f, 400.0f);
	MaxRange = FMath::RandRange(200.0f, 800.0f);

	const int32 DirectionIndex = FMath::RandRange(0, 2);

	if (DirectionIndex == 0)
	{
		MoveDirection = FVector(1.0f, 0.0f, 0.0f);
	}
	else if (DirectionIndex == 1)
	{
		MoveDirection = FVector(0.0f, 1.0f, 0.0f);
	}
	else
	{
		MoveDirection = FVector(0.0f, 0.0f, 1.0f);
	}

	CurrentDistance = 0.0f;
	MoveDirectionSign = 1;
}