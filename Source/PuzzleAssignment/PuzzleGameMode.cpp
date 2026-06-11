#include "PuzzleGameMode.h"
#include "PuzzlePawn.h"

APuzzleGameMode::APuzzleGameMode()
{
	DefaultPawnClass = APuzzlePawn::StaticClass();
}