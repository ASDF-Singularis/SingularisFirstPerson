#include "Components/SingularisCameraSmoothComponent.h"

#include <GameFramework/Character.h>

USingularisCameraSmoothComponent::USingularisCameraSmoothComponent()
{
	SetIsReplicatedByDefault(false);

	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;

	bAutoActivate = true;
}

void USingularisCameraSmoothComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ACharacter>(), TEXT("SingularisCameraSmoothComponent: Owner not is Character"));

	Character = Cast<ACharacter>(GetOwner());
}

void USingularisCameraSmoothComponent::TickComponent(
	const float DeltaTime,
	const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Character.IsValid() || !Character->IsLocallyControlled()) return;

	if (!IsActive()) return;

	// 使用FInterpTo进行平滑插值
	CurrentLookAxisVector.X = FMath::FInterpTo(
		CurrentLookAxisVector.X,
		TargetLookAxisVector.X,
		DeltaTime,
		LookInterpSpeed
	);
	CurrentLookAxisVector.Y = FMath::FInterpTo(
		CurrentLookAxisVector.Y,
		TargetLookAxisVector.Y,
		DeltaTime,
		LookInterpSpeed
	);

	// 应用插值后的输入
	Character->AddControllerYawInput(CurrentLookAxisVector.X);
	Character->AddControllerPitchInput(CurrentLookAxisVector.Y);

	// 可选：重置目标值以减少惯性效应
	TargetLookAxisVector = FVector2D::ZeroVector;
}

void USingularisCameraSmoothComponent::SetLookAxisVector(const FVector2D& LookAxisVector)
{
	TargetLookAxisVector = LookAxisVector;
}
