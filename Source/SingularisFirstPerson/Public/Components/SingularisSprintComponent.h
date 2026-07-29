#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "SingularisSprintComponent.generated.h"

/**
 * 引力奇点冲刺组件
 * 
 * Client Prediction and Server Reconciliation Multiplayer 设计模式典范
 */
UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = ("Singularis"),
	meta = (BlueprintSpawnableComponent, DisplayName = "引力奇点冲刺组件")
)
class SINGULARISFIRSTPERSON_API USingularisSprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:
#pragma region Parameter

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点冲刺组件|参数",
		meta = (DisplayName = "步行速度")
	)
	float WalkSpeed = 200.0f;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "SingularisFirstPerson|引力奇点冲刺组件|参数",
		meta = (DisplayName = "冲刺速率")
	)
	float SprintSpeedRatio = 3.0f;

#pragma endregion

private:
#pragma region Internal Variable

	TWeakObjectPtr<ACharacter> Character = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_IsSprinting)
	bool bIsSprinting = false;

#pragma endregion

public:
#pragma region Constructors

	USingularisSprintComponent();

#pragma endregion

#pragma region ActorComponent Interface

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion

#pragma region State

	UFUNCTION(
		BlueprintCallable,
		Category = "SingularisFirstPerson|引力奇点冲刺组件|State",
		meta = (DisplayName = "正在冲刺")
	)
	bool IsSprinting() const { return bIsSprinting; }

#pragma endregion

#pragma region API

	UFUNCTION(
		BlueprintCallable,
		Category = "SingularisFirstPerson|引力奇点冲刺组件|API",
		meta = (DisplayName = "冲刺开始")
	)
	void SprintStarted();

	UFUNCTION(
		BlueprintCallable,
		Category = "SingularisFirstPerson|引力奇点冲刺组件|API",
		meta = (DisplayName = "冲刺完成")
	)
	void SprintCompleted();

#pragma endregion

private:
#pragma region RPC

	UFUNCTION(Server, Reliable)
	void Server_SprintStarted();

	UFUNCTION(Server, Reliable)
	void Server_SprintCompleted();

#pragma endregion

#pragma region Response

	UFUNCTION()
	void OnRep_IsSprinting() const;

#pragma endregion

#pragma region Callback

#pragma endregion

#pragma region Internal Function

	void Setup() const;

	void ApplyIsSprinting() const;

#pragma endregion
};
