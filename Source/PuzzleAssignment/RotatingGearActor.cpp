#include "RotatingGearActor.h"

ARotatingGearActor::ARotatingGearActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	RotationSpeed = FRotator(0.0f, 0.0f, 90.0f);
}

void ARotatingGearActor::BeginPlay()
{
	Super::BeginPlay();
}

void ARotatingGearActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(RotationSpeed * DeltaTime);
}