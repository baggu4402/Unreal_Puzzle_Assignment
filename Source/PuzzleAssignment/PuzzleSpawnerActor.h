#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleSpawnerActor.generated.h"

class ARotatingGearActor;
class AMovingPlatformActor;

UCLASS()
class PUZZLEASSIGNMENT_API APuzzleSpawnerActor : public AActor
{
	GENERATED_BODY()

public:
	APuzzleSpawnerActor();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnRandomPuzzles();

	FVector GetRandomSpawnLocation() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ARotatingGearActor> RotatingActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AMovingPlatformActor> MovingPlatformClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 RotatingActorCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MovingPlatformCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FVector SpawnAreaMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FVector SpawnAreaMax;
};