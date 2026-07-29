#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "SingularisCharacterControllerComponent.generated.h"

struct FInputActionValue;
class ACharacter;
class USingularisMoveSmoothComponent;
class USingularisCameraSmoothComponent;
class USingularisSprintComponent;
class UInputAction;
class UInputMappingContext;

UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = ("Singularis"),
	meta = (BlueprintSpawnableComponent, DisplayName = "引力奇点角色控制器组件")
)
class SINGULARISFIRSTPERSON_API USingularisCharacterControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#pragma region Parameter

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|参数",
		meta = (DisplayName = "自动控制")
	)
	bool bAutoControl = true;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|参数",
		meta = (DisplayName = "启动启用")
	)
	bool bStartEnabled = true;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|参数",
		meta = (DisplayName = "摄像机灵敏度")
	)
	float CameraSensitivity = 1.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|输入",
		meta = (DisplayName = "输入优先级")
	)
	int32 InputPriority = 10;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|输入",
		meta = (DisplayName = "输入映射上下文")
	)
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|输入",
		meta = (DisplayName = "视角输入动作")
	)
	UInputAction* LookInputAction = nullptr;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|输入",
		meta = (DisplayName = "移动输入动作")
	)
	UInputAction* MoveInputAction = nullptr;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|输入",
		meta = (DisplayName = "跳跃输入动作")
	)
	UInputAction* JumpInputAction = nullptr;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|输入",
		meta = (DisplayName = "冲刺输入动作")
	)
	UInputAction* SprintInputAction = nullptr;

#pragma endregion

private:
#pragma region Internal Variable

	TWeakObjectPtr<APlayerController> OwnerPlayerController = nullptr;
	TWeakObjectPtr<ACharacter> ControlledCharacter = nullptr;

	bool bIsEnabled = false;

#pragma endregion

public:
#pragma region Constructors

	USingularisCharacterControllerComponent();

#pragma endregion

#pragma region ActorComponent Interface

	virtual void BeginPlay() override;

#pragma endregion

#pragma region API

	UFUNCTION(
		BlueprintCallable,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|API",
		meta = (DisplayName = "SetEnabled")
	)
	void SetEnabled(bool IsEnabled);

	UFUNCTION(
		BlueprintCallable,
		Category = "SingularisFirstPerson|引力奇点角色控制器组件|API",
		meta = (DisplayName = "SetControlled")
	)
	void SetControlled(ACharacter* Character);

#pragma endregion

private:
#pragma region Internal Function

	void BindInputAction();
	void RefreshInputMappingContext() const;

#pragma endregion

#pragma region Callback

	void HandleJump();
	void HandleStopJumping();
	void HandleMoveInputAction(const FInputActionValue& Value);
	void HandleLookInputAction(const FInputActionValue& Value);
	void HandleSprintInputActionStarted(const FInputActionValue& Value);
	void HandleSprintInputActionCompleted(const FInputActionValue& Value);

	UFUNCTION()
	void OnPossessPawnChanged(APawn* OldPawn, APawn* NewPawn);

#pragma endregion
};
