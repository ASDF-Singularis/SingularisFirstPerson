#include "Actors/SingularisLoFiCharacter.h"

#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <UObject/ConstructorHelpers.h>

ASingularisLoFiCharacter::ASingularisLoFiCharacter()
{
	bReplicates = true;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	static const ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(
		TEXT(
			"/SingularisFirstPerson/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"
		)
	);

	static ConstructorHelpers::FClassFinder<UAnimInstance> UnarmedAnimInstanceClassFinder(
		TEXT(
			"/SingularisFirstPerson/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C"
		)
	);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 15.0;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	if (UnarmedAnimInstanceClassFinder.Succeeded())
		GetMesh()->SetAnimInstanceClass(UnarmedAnimInstanceClassFinder.Class);
	if (SkeletalMeshFinder.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(SkeletalMeshFinder.Object);
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCollisionProfileName(FName("CharacterMesh"));
}

void ASingularisLoFiCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASingularisLoFiCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
