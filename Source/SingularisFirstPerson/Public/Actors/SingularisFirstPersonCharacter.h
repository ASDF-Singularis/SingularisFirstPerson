#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "SingularisFirstPersonCharacter.generated.h"

class UCameraComponent;

UCLASS(Abstract, Blueprintable)
class SINGULARISFIRSTPERSON_API ASingularisFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

#pragma region Constructors

	ASingularisFirstPersonCharacter();

#pragma endregion

#pragma region Actor Interface

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

#pragma endregion
};
