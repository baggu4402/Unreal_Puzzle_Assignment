#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatformActor.generated.h"

UCLASS()
class PUZZLEASSIGNMENT_API AMovingPlatformActor : public AActor
{
	GENERATED_BODY()

public:
	AMovingPlatformActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetMoveSettings(float NewMoveSpeed, float NewMaxRange, const FVector& NewMoveDirection);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MoveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool bUseRandomTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float RandomizeInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float RandomPositionRange;

private:
	FVector OriginalLocation;

	FVector StartLocation;

	float CurrentDistance;

	int32 MoveDirectionSign;

	FTimerHandle RandomMoveTimerHandle;

	void RandomizeMovement();
};