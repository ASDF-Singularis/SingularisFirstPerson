#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "SingularisCameraDynamicFOVComponent.generated.h"

UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = ("Singularis"),
	meta = (BlueprintSpawnableComponent, DisplayName = "引力奇点摄像机动态 FOV 组件")
)
class SINGULARISFIRSTPERSON_API USingularisCameraDynamicFOVComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#pragma region Parameter

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机动态 FOV 组件|参数",
		meta = (DisplayName = "默认 FOV")
	)
	float DefaultFOV = 103.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机动态 FOV 组件|参数",
		meta = (DisplayName = "动态 FOV")
	)
	float DynamicFOV = 20.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机动态 FOV 组件|参数",
		meta = (DisplayName = "插值速度", ClampMin = 0.0f, ClampMax = 20.0f)
	)
	float InterpSpeed = 8.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机动态 FOV 组件|参数",
		meta = (DisplayName = "最小触发速度")
	)
	float MinTriggerSpeed = 100.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机动态 FOV 组件|参数",
		meta = (DisplayName = "最大触发速度", ClampMin = 0.0)
	)
	float MaxTriggerSpeed = 600.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点摄像机动态 FOV 组件|参数",
		meta = (DisplayName = "FOV 变化曲线")
	)
	UCurveFloat* FOVCurve = nullptr;

#pragma endregion

private:
#pragma region Internal Variable

	TWeakObjectPtr<ACharacter> Character = nullptr;
	float CurrentFOV = 90.0f;
	float TargetFOV = 90.0f;

#pragma endregion

public:
#pragma region Constructors

	USingularisCameraDynamicFOVComponent();

#pragma endregion

#pragma region ActorComponent Interface

	virtual void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

#pragma endregion

#pragma region Internal Function

	void ResetFOVImmediately();
	void SetDefaultFOV(float NewDefaultFOV, bool bImmediate = false);
	float CalculateTargetFOV(float CurrentSpeed) const;

#pragma endregion
};
