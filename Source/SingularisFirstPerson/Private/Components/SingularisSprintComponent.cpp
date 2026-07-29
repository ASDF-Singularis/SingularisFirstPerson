#include "Components/SingularisSprintComponent.h"

#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Net/UnrealNetwork.h>

USingularisSprintComponent::USingularisSprintComponent()
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = true;
}

void USingularisSprintComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ACharacter>(), TEXT("SingularisCameraDynamicFOVComponent: Owner not is Character"));

	Character = Cast<ACharacter>(GetOwner());

	Setup();
}

void USingularisSprintComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USingularisSprintComponent, bIsSprinting);
}

void USingularisSprintComponent::SprintStarted()
{
	// 1) 本地控制
	if (!Character.IsValid() || !Character->IsLocallyControlled()) return;

	// 2) 幂等性
	if (bIsSprinting == true) return;

	// 3) 设置本地状态
	bIsSprinting = true;

	// 4) 本地应用副作用
	//    响应式编程
	ApplyIsSprinting();

	// 5) 通知服务器
	Server_SprintStarted();
}

void USingularisSprintComponent::SprintCompleted()
{
	// 1) 本地检查
	if (!Character.IsValid() || !Character->IsLocallyControlled()) return;

	// 2) 幂等性
	if (bIsSprinting == false) return;

	// 3) 设置本地状态
	bIsSprinting = false;

	// 4) 本地应用副作用
	//    响应式编程
	ApplyIsSprinting();

	// 5) 通知服务器
	Server_SprintCompleted();
}

void USingularisSprintComponent::Setup() const
{
	if (!Character.IsValid()) return;

	Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void USingularisSprintComponent::Server_SprintStarted_Implementation()
{
	// 1) 服务器权威
	if (!Character.IsValid() || !Character->HasAuthority()) return;

	// 2) 幂等性
	if (bIsSprinting == true) return;

	// 3) 设置复制副作用状态
	bIsSprinting = true;

	// 4) 服务器应用副作用
	//    响应式编程
	ApplyIsSprinting();
}

void USingularisSprintComponent::Server_SprintCompleted_Implementation()
{
	// 1) 服务器权威
	if (!Character.IsValid() || !Character->HasAuthority()) return;

	// 2) 幂等性
	if (bIsSprinting == false) return;

	// 3) 设置复制副作用状态
	bIsSprinting = false;

	// 4) 服务器应用副作用
	//    响应式编程
	ApplyIsSprinting();
}

void USingularisSprintComponent::OnRep_IsSprinting() const
{
	ApplyIsSprinting();
}

void USingularisSprintComponent::ApplyIsSprinting() const
{
	if (!Character.IsValid()) return;

	Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * (IsSprinting() ? SprintSpeedRatio : 1);
}
