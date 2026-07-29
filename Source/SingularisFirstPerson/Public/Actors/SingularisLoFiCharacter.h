#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "SingularisLoFiCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract, Blueprintable)
class SINGULARISFIRSTPERSON_API ASingularisLoFiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

#pragma region Constructors

	ASingularisLoFiCharacter();

#pragma endregion

#pragma region Actor Interface

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

#pragma endregion
};
