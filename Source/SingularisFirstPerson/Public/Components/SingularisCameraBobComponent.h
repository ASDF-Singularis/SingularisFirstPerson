#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "SingularisCameraBobComponent.generated.h"

class ACharacter;

UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = ("Singularis"),
	meta = (BlueprintSpawnableComponent, DisplayName = "引力奇点摄像机 BOB 组件")
)
class SINGULARISFIRSTPERSON_API USingularisCameraBobComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#pragma region Parameter

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机 BOB 组件|参数",
		meta = (DisplayName = "BOB 振幅")
	)
	float BobAmplitude = 4.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机 BOB 组件|参数",
		meta = (DisplayName = "BOB 频率")
	)
	float BobFrequency = 0.05f;

#pragma endregion

private:
#pragma region Internal Variable

	TWeakObjectPtr<ACharacter> Character = nullptr;

	FVector InitialCameraLocation{};
	float CurrentBOBTime = 0.0f;

#pragma endregion

public:
#pragma region Constructors

	USingularisCameraBobComponent();

#pragma endregion

#pragma region ActorComponent Interface

	virtual void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

#pragma endregion
};
