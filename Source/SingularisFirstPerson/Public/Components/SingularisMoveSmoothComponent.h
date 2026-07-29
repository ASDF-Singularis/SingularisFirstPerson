#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "SingularisMoveSmoothComponent.generated.h"

UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = ("Singularis"),
	meta = (BlueprintSpawnableComponent, DisplayName = "引力奇点移动平滑组件")
)
class SINGULARISFIRSTPERSON_API USingularisMoveSmoothComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#pragma region ActorComponent Interface

	virtual void BeginPlay() override;

#pragma endregion
};
