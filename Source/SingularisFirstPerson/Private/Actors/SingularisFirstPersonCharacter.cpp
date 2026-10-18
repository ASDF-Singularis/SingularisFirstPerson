#include "Actors/SingularisFirstPersonCharacter.h"

#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <UObject/ConstructorHelpers.h>

ASingularisFirstPersonCharacter::ASingularisFirstPersonCharacter()
{
	bReplicates = true;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(
		TEXT(
			"/SingularisFirstPerson/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"
		)
	);

	static ConstructorHelpers::FClassFinder<UAnimInstance> UnarmedAnimInstanceClassFinder(
		TEXT(
			"/SingularisFirstPerson/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C"
		)
	);

	static ConstructorHelpers::FClassFinder<UAnimInstance> FirstPersonAnimInstanceClassFinder(
		TEXT(
			"/SingularisFirstPerson/Anims/ABP_FP_Copy.ABP_FP_Copy_C"
		)
	);

	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMeshComponent"));
	FirstPersonMeshComponent->SetupAttachment(GetMesh());
	if (FirstPersonAnimInstanceClassFinder.Succeeded())
		FirstPersonMeshComponent->SetAnimInstanceClass(FirstPersonAnimInstanceClassFinder.Class);
	if (SkeletalMeshFinder.Succeeded())
		FirstPersonMeshComponent->SetSkeletalMeshAsset(SkeletalMeshFinder.Object);
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));
	SpringArmComponent->SetRelativeLocation(FVector(-2.8f, 5.89f, 0.0f));
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = false;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 15.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bEnableFirstPersonFieldOfView = true;
	CameraComponent->bEnableFirstPersonScale = true;
	CameraComponent->FirstPersonFieldOfView = 90.0f;
	CameraComponent->FirstPersonScale = 0.6f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(-20.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));
	if (UnarmedAnimInstanceClassFinder.Succeeded())
		GetMesh()->SetAnimInstanceClass(UnarmedAnimInstanceClassFinder.Class);
	if (SkeletalMeshFinder.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(SkeletalMeshFinder.Object);
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	// 第三人称网格是 ABP_FP_Copy 的姿态源，第一人称网格承载摄像机挂点；
	// 两者都必须在不可见时继续求值，否则姿态及其骨骼变换会冻结。
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	FirstPersonMeshComponent->VisibilityBasedAnimTickOption =
		EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ASingularisFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASingularisFirstPersonCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
