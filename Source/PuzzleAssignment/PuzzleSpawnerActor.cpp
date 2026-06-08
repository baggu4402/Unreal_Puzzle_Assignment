#include "PuzzleSpawnerActor.h"
#include "RotatingGearActor.h"
#include "MovingPlatformActor.h"
#include "Engine/World.h"

APuzzleSpawnerActor::APuzzleSpawnerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RotatingActorCount = 5;
	MovingPlatformCount = 5;

	SpawnAreaMin = FVector(-1000.0f, -1000.0f, 100.0f);
	SpawnAreaMax = FVector(1000.0f, 1000.0f, 500.0f);
}

void APuzzleSpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnRandomPuzzles();
}

void APuzzleSpawnerActor::SpawnRandomPuzzles()
{
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	for (int32 i = 0; i < RotatingActorCount; i++)
	{
		if (RotatingActorClass == nullptr)
		{
			continue;
		}

		const FVector SpawnLocation = GetRandomSpawnLocation();
		const FRotator SpawnRotation = FRotator::ZeroRotator;

		ARotatingGearActor* SpawnedActor = World->SpawnActor<ARotatingGearActor>(
			RotatingActorClass,
			SpawnLocation,
			SpawnRotation
		);

		if (SpawnedActor != nullptr)
		{
			const float RandomPitch = FMath::RandRange(0.0f, 180.0f);
			const float RandomYaw = FMath::RandRange(60.0f, 240.0f);
			const float RandomRoll = FMath::RandRange(0.0f, 180.0f);

			SpawnedActor->SetRotationSpeed(FRotator(RandomPitch, RandomYaw, RandomRoll));
		}
	}

	for (int32 i = 0; i < MovingPlatformCount; i++)
	{
		if (MovingPlatformClass == nullptr)
		{
			continue;
		}

		const FVector SpawnLocation = GetRandomSpawnLocation();
		const FRotator SpawnRotation = FRotator::ZeroRotator;

		AMovingPlatformActor* SpawnedActor = World->SpawnActor<AMovingPlatformActor>(
			MovingPlatformClass,
			SpawnLocation,
			SpawnRotation
		);

		if (SpawnedActor != nullptr)
		{
			const float RandomMoveSpeed = FMath::RandRange(100.0f, 400.0f);
			const float RandomMaxRange = FMath::RandRange(200.0f, 800.0f);

			const int32 DirectionIndex = FMath::RandRange(0, 2);

			FVector RandomDirection = FVector(1.0f, 0.0f, 0.0f);

			if (DirectionIndex == 0)
			{
				RandomDirection = FVector(1.0f, 0.0f, 0.0f);
			}
			else if (DirectionIndex == 1)
			{
				RandomDirection = FVector(0.0f, 1.0f, 0.0f);
			}
			else
			{
				RandomDirection = FVector(0.0f, 0.0f, 1.0f);
			}

			SpawnedActor->SetMoveSettings(RandomMoveSpeed, RandomMaxRange, RandomDirection);
		}
	}
}

FVector APuzzleSpawnerActor::GetRandomSpawnLocation() const
{
	const float RandomX = FMath::RandRange(SpawnAreaMin.X, SpawnAreaMax.X);
	const float RandomY = FMath::RandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);
	const float RandomZ = FMath::RandRange(SpawnAreaMin.Z, SpawnAreaMax.Z);

	return FVector(RandomX, RandomY, RandomZ);
}