#include "Components/SingularisCameraDynamicFOVComponent.h"

#include <Camera/CameraComponent.h>
#include <Curves/CurveFloat.h>
#include <GameFramework/Character.h>
#include <GameFramework/PawnMovementComponent.h>

USingularisCameraDynamicFOVComponent::USingularisCameraDynamicFOVComponent()
{
	SetIsReplicatedByDefault(false);

	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;

	bAutoActivate = true;
}

void USingularisCameraDynamicFOVComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ACharacter>(), TEXT("SingularisCameraDynamicFOVComponent: Owner not is Character"));

	Character = Cast<ACharacter>(GetOwner());
}

void USingularisCameraDynamicFOVComponent::TickComponent(
	const float DeltaTime,
	const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Character.IsValid() || !Character->IsLocallyControlled()) return;

	if (!IsActive()) return;

	UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>();
	if (!CameraComponent || !Character->GetMovementComponent()) return;

	// 计算当前速度
	const FVector Velocity = Character->GetMovementComponent()->Velocity;
	const float CurrentSpeed = Velocity.Size2D(); // 使用水平速度，忽略垂直移动

	// 计算目标FOV
	TargetFOV = CalculateTargetFOV(CurrentSpeed);

	// 平滑插值当前FOV
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, InterpSpeed);

	// 应用FOV到摄像机
	CameraComponent->SetFieldOfView(CurrentFOV);
}

void USingularisCameraDynamicFOVComponent::ResetFOVImmediately()
{
	if (!Character.IsValid() || !Character->IsLocallyControlled()) return;

	CurrentFOV = DefaultFOV;
	TargetFOV = DefaultFOV;

	if (Character.IsValid())
	{
		if (UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>())
			CameraComponent->SetFieldOfView(DefaultFOV);
	}
}

void USingularisCameraDynamicFOVComponent::SetDefaultFOV(const float NewDefaultFOV, const bool bImmediate)
{
	DefaultFOV = NewDefaultFOV;

	if (bImmediate)
		ResetFOVImmediately();
}

float USingularisCameraDynamicFOVComponent::CalculateTargetFOV(const float CurrentSpeed) const
{
	// 如果速度低于最小触发值，使用默认FOV
	if (CurrentSpeed <= MinTriggerSpeed)
		return DefaultFOV;

	// 计算速度在最小和最大触发速度之间的归一化值
	const float SpeedAlpha = FMath::Clamp(
		(CurrentSpeed - MinTriggerSpeed) / (MaxTriggerSpeed - MinTriggerSpeed),
		0.0f,
		1.0f
	);

	// 使用曲线或线性插值计算FOV强度
	const float FOVIntensity = FOVCurve ? FOVCurve->GetFloatValue(SpeedAlpha) : SpeedAlpha;

	// 计算最终目标FOV
	return DefaultFOV + DynamicFOV * FOVIntensity;
}
