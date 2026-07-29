#include "Actors/SingularisFirstPersonPlayerController.h"

#include "Components/SingularisCharacterControllerComponent.h"

ASingularisFirstPersonPlayerController::ASingularisFirstPersonPlayerController()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	CharacterControllerComponent = CreateDefaultSubobject<USingularisCharacterControllerComponent>(
		TEXT("CharacterControllerComponent")
	);
}
