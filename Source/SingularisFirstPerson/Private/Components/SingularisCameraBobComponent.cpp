#include "Components/SingularisCameraBobComponent.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/Character.h>
#include <GameFramework/PawnMovementComponent.h>

USingularisCameraBobComponent::USingularisCameraBobComponent()
{
	SetIsReplicatedByDefault(false);

	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;

	bAutoActivate = true;
}

void USingularisCameraBobComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ACharacter>(), TEXT("SingularisCameraBobComponent: Owner not is Character"));

	Character = Cast<ACharacter>(GetOwner());

	if (const auto CameraComponent = Character->FindComponentByClass<UCameraComponent>())
		InitialCameraLocation = CameraComponent->GetRelativeLocation();
}

void USingularisCameraBobComponent::TickComponent(
	const float DeltaTime,
	const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Character.IsValid() || !Character->IsLocallyControlled()) return;

	if (!IsActive()) return;

	UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>();
	if (!CameraComponent || !Character->GetMovementComponent() || Character->GetMovementComponent()->IsFalling())
		return;

	// 根据移动状态计算强度
	const float Speed = Character->GetMovementComponent()->Velocity.Size();

	// 使用曲线控制运动轨迹	
	CurrentBOBTime += DeltaTime * Speed * BobFrequency;
	const float XOffset = FMath::Cos(CurrentBOBTime / 2.0f) * BobAmplitude;
	const float YOffset = FMath::Sin(CurrentBOBTime) * BobAmplitude;

	// 应用位置偏移
	const auto NewLocation = InitialCameraLocation + FVector(0.0f, XOffset, YOffset);
	CameraComponent->SetRelativeLocation(NewLocation);
}
