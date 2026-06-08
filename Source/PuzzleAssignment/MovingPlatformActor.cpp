#include "MovingPlatformActor.h"

AMovingPlatformActor::AMovingPlatformActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	MoveSpeed = 200.0f;
	MaxRange = 500.0f;
	MoveDirection = FVector(1.0f, 0.0f, 0.0f);

	CurrentDistance = 0.0f;
	MoveDirectionSign = 1;
}

void AMovingPlatformActor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	MoveDirection = MoveDirection.GetSafeNormal();
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