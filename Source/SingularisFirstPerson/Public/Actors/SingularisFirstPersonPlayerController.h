#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerController.h>

#include "SingularisFirstPersonPlayerController.generated.h"

class USingularisCharacterControllerComponent;

UCLASS(Abstract, Blueprintable)
class SINGULARISFIRSTPERSON_API ASingularisFirstPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USingularisCharacterControllerComponent> CharacterControllerComponent = nullptr;

#pragma region Constructors

	ASingularisFirstPersonPlayerController();

#pragma endregion
};
