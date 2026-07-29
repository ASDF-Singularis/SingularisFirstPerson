#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "SingularisCameraSmoothComponent.generated.h"

class ACharacter;

UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = ("Singularis"),
	meta = (BlueprintSpawnableComponent, DisplayName = "引力奇点摄像机平滑组件")
)
class SINGULARISFIRSTPERSON_API USingularisCameraSmoothComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#pragma region Parameter

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机平滑组件|参数",
		meta = (DisplayName = "Look 插值速度")
	)
	float LookInterpSpeed = 12.0f;

#pragma endregion

private:
#pragma region Internal Variable

	TWeakObjectPtr<ACharacter> Character = nullptr;

	FVector2D CurrentLookAxisVector{};
	FVector2D TargetLookAxisVector{};

#pragma endregion

public:
#pragma region Constructors

	USingularisCameraSmoothComponent();

#pragma endregion

#pragma region ActorComponent Interface

	virtual void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

#pragma endregion

#pragma region API

	UFUNCTION(
		BlueprintCallable,
		Category = "SingularisFirstPerson|引力奇点摄像机平滑组件|API",
		meta = (DisplayName = "设置视轴向量")
	)
	void SetLookAxisVector(const FVector2D& LookAxisVector);

#pragma endregion
};
