#include "Components/SingularisCharacterControllerComponent.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputMappingContext.h>
#include <Engine/LocalPlayer.h>
#include <GameFramework/Character.h>
#include <UObject/ConstructorHelpers.h>

#include "Components/SingularisCameraSmoothComponent.h"
#include "Components/SingularisSprintComponent.h"

USingularisCharacterControllerComponent::USingularisCharacterControllerComponent()
{
	SetIsReplicatedByDefault(false);

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = true;

	static const ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultInputMappingContextFinder(
		TEXT("/SingularisFirstPerson/Inputs/IMC_Singularis_Character.IMC_Singularis_Character")
	);
	static const ConstructorHelpers::FObjectFinder<UInputAction> DefaultLookInputActionFinder(
		TEXT("/SingularisFirstPerson/Inputs/Actions/IA_Singularis_Look.IA_Singularis_Look")
	);
	static const ConstructorHelpers::FObjectFinder<UInputAction> DefaultMoveInputActionFinder(
		TEXT("/SingularisFirstPerson/Inputs/Actions/IA_Singularis_Move.IA_Singularis_Move")
	);
	static const ConstructorHelpers::FObjectFinder<UInputAction> DefaultJumpInputActionFinder(
		TEXT("/SingularisFirstPerson/Inputs/Actions/IA_Singularis_Jump.IA_Singularis_Jump")
	);
	static const ConstructorHelpers::FObjectFinder<UInputAction> DefaultSprintInputActionFinder(
		TEXT("/SingularisFirstPerson/Inputs/Actions/IA_Singularis_Sprint.IA_Singularis_Sprint")
	);

	if (DefaultInputMappingContextFinder.Succeeded())
		InputMappingContext = DefaultInputMappingContextFinder.Object;
	if (DefaultLookInputActionFinder.Succeeded())
		LookInputAction = DefaultLookInputActionFinder.Object;
	if (DefaultMoveInputActionFinder.Succeeded())
		MoveInputAction = DefaultMoveInputActionFinder.Object;
	if (DefaultJumpInputActionFinder.Succeeded())
		JumpInputAction = DefaultJumpInputActionFinder.Object;
	if (DefaultSprintInputActionFinder.Succeeded())
		SprintInputAction = DefaultSprintInputActionFinder.Object;
}

void USingularisCharacterControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(
		GetOwner()->IsA<APlayerController>(),
		TEXT("SingularisCharacterControllerComponent: Owner not is PlayerController")
	);

	OwnerPlayerController = Cast<APlayerController>(GetOwner());

	BindInputAction();

	if (OwnerPlayerController.IsValid() && OwnerPlayerController->IsLocalController())
	{
		if (bAutoControl)
		{
			OwnerPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessPawnChanged);

			SetControlled(OwnerPlayerController->GetCharacter());
		}

		if (bStartEnabled)
			SetEnabled(true);
	}

	/*if (ControlledCharacter.IsValid())
	// 延迟一帧确保 ControlledCharacter 已设置
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USingularisCharacterControllerComponent::BindInputAction);*/
}

void USingularisCharacterControllerComponent::SetEnabled(const bool IsEnabled)
{
	// 本地控制检查
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;

	// 幂等性
	if (bIsEnabled == IsEnabled) return;
	bIsEnabled = IsEnabled;

	RefreshInputMappingContext();
}

void USingularisCharacterControllerComponent::SetControlled(ACharacter* Character)
{
	// 本地控制检查
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;

	if (ControlledCharacter == Character) return;
	ControlledCharacter = Character;

	RefreshInputMappingContext();
}

void USingularisCharacterControllerComponent::SetSensitivity(float Sensitivity)
{
	CameraSensitivity = Sensitivity;
}

void USingularisCharacterControllerComponent::BindInputAction()
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;

	// 绑定输入动作
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
		OwnerPlayerController->InputComponent
	);
	if (!IsValid(EnhancedInputComponent)) return;

	// Jumping
	EnhancedInputComponent->BindAction(
		JumpInputAction,
		ETriggerEvent::Started,
		this,
		&USingularisCharacterControllerComponent::HandleJump
	);
	EnhancedInputComponent->BindAction(
		JumpInputAction,
		ETriggerEvent::Completed,
		this,
		&USingularisCharacterControllerComponent::HandleStopJumping
	);

	// Moving
	EnhancedInputComponent->BindAction(
		MoveInputAction,
		ETriggerEvent::Triggered,
		this,
		&USingularisCharacterControllerComponent::HandleMoveInputAction
	);

	// Looking
	EnhancedInputComponent->BindAction(
		LookInputAction,
		ETriggerEvent::Triggered,
		this,
		&USingularisCharacterControllerComponent::HandleLookInputAction
	);

	// Spring
	EnhancedInputComponent->BindAction(
		SprintInputAction,
		ETriggerEvent::Started,
		this,
		&USingularisCharacterControllerComponent::HandleSprintInputActionStarted
	);
	EnhancedInputComponent->BindAction(
		SprintInputAction,
		ETriggerEvent::Completed,
		this,
		&USingularisCharacterControllerComponent::HandleSprintInputActionCompleted
	);
}

void USingularisCharacterControllerComponent::RefreshInputMappingContext() const
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;
	if (!IsValid(InputMappingContext)) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwnerPlayerController->GetLocalPlayer());
	if (!IsValid(Subsystem)) return;

	if (ControlledCharacter.IsValid() && bIsEnabled)
		Subsystem->AddMappingContext(InputMappingContext, InputPriority);
	else
		Subsystem->RemoveMappingContext(InputMappingContext);
}

// ReSharper disable CppMemberFunctionMayBeConst

void USingularisCharacterControllerComponent::HandleJump()
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;
	if (!ControlledCharacter.IsValid()) return;

	ControlledCharacter->Jump();
}

void USingularisCharacterControllerComponent::HandleStopJumping()
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;
	if (!ControlledCharacter.IsValid()) return;

	ControlledCharacter->StopJumping();
}

void USingularisCharacterControllerComponent::HandleMoveInputAction(const FInputActionValue& Value)
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;
	if (!ControlledCharacter.IsValid()) return;

	const auto MovementVector = Value.Get<FVector2D>();

	if (ControlledCharacter->GetController())
	{
		ControlledCharacter->AddMovementInput(ControlledCharacter->GetActorForwardVector(), MovementVector.Y);
		ControlledCharacter->AddMovementInput(ControlledCharacter->GetActorRightVector(), MovementVector.X);
	}
}

void USingularisCharacterControllerComponent::HandleLookInputAction(const FInputActionValue& Value)
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;
	if (!ControlledCharacter.IsValid()) return;

	USingularisCameraSmoothComponent* CameraSmoothComponent = ControlledCharacter->FindComponentByClass<
		USingularisCameraSmoothComponent>();

	const auto LookAxisVector = Value.Get<FVector2D>() * CameraSensitivity;

	if (IsValid(CameraSmoothComponent))
		CameraSmoothComponent->SetLookAxisVector(LookAxisVector);
	else
	{
		ControlledCharacter->AddControllerYawInput(LookAxisVector.X);
		ControlledCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void USingularisCharacterControllerComponent::HandleSprintInputActionStarted(const FInputActionValue& Value)
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;
	if (!ControlledCharacter.IsValid()) return;

	USingularisSprintComponent* SprintComponent = ControlledCharacter->FindComponentByClass<
		USingularisSprintComponent>();
	if (!IsValid(SprintComponent)) return;

	SprintComponent->SprintStarted();
}

void USingularisCharacterControllerComponent::HandleSprintInputActionCompleted(const FInputActionValue& Value)
{
	if (!OwnerPlayerController.IsValid() || !OwnerPlayerController->IsLocalController()) return;
	if (!ControlledCharacter.IsValid()) return;

	USingularisSprintComponent* SprintComponent = ControlledCharacter->FindComponentByClass<
		USingularisSprintComponent>();
	if (!IsValid(SprintComponent)) return;

	SprintComponent->SprintCompleted();
}

void USingularisCharacterControllerComponent::OnPossessPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (!IsValid(NewPawn)) return;

	ACharacter* Character = Cast<ACharacter>(NewPawn);
	if (!IsValid(Character)) return;

	SetControlled(Character);
}

// ReSharper restore CppMemberFunctionMayBeConst
