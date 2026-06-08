#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingGearActor.generated.h"

UCLASS()
class PUZZLEASSIGNMENT_API ARotatingGearActor : public AActor
{
	GENERATED_BODY()

public:
	ARotatingGearActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void SetRotationSpeed(const FRotator& NewRotationSpeed);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationSpeed;
};