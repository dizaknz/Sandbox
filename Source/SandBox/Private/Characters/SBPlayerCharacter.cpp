#include "Characters/SBPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputActionData.h"

ASBPlayerCharacter::ASBPlayerCharacter()
{
	// location and rotation of the character mesh transform
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -90.0f), 
		FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// attach spring arm to the default character's skeletal mesh component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->bUsePawnControlRotation = true;

	// attach camera to spring arm
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCompent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// do not rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// character movement component defaults
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->NavAgentProps.bCanFly = true;
	GetCharacterMovement()->NavAgentProps.bCanSwim = true;

	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//PrimaryActorTick.bCanEverTick = true;

	SprintSpeed = 1200.0f;
	WalkSpeed = 600.0f;
	JumpSpeed = 100.0f;
}

void ASBPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpSpeed;

}

void ASBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}
	const FVector2D MoveValue = Value.Get<FVector2D>();
	const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

	// Forward/Backward direction
	if (MoveValue.Y != 0.f)
	{
		const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction, MoveValue.Y);
	}

	// Right/Left direction
	if (MoveValue.X != 0.f)
	{
		const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(Direction, MoveValue.X);
	}
}

void ASBPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}
	const FVector2D LookValue = Value.Get<FVector2D>();

	if (LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X);
	}

	if (LookValue.Y != 0.f)
	{
		AddControllerPitchInput(LookValue.Y);
	}
}

void ASBPlayerCharacter::OnJump(const FInputActionValue& Value)
{
	bool bJump = Value.Get<bool>();
	if (bJump)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void ASBPlayerCharacter::OnCrouch(const FInputActionValue& Value)
{
	bool bCrouch = Value.Get<bool>();
	if (bCrouch)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ASBPlayerCharacter::OnSprint(const FInputActionValue& Value)
{
	bool bSprint = Value.Get<bool>();
	if (bSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ASBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(InputMapping);
	check(InputActions);
	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this, &ASBPlayerCharacter::Move);
	PEI->BindAction(InputActions->LookAction, ETriggerEvent::Triggered, this, &ASBPlayerCharacter::Look);
	PEI->BindAction(InputActions->JumpAction, ETriggerEvent::Started, this, &ASBPlayerCharacter::OnJump);
	PEI->BindAction(InputActions->JumpAction, ETriggerEvent::Completed, this, &ASBPlayerCharacter::OnJump);
	PEI->BindAction(InputActions->CrouchAction, ETriggerEvent::Started, this, &ASBPlayerCharacter::OnCrouch);
	PEI->BindAction(InputActions->CrouchAction, ETriggerEvent::Completed, this, &ASBPlayerCharacter::OnCrouch);
	PEI->BindAction(InputActions->SprintAction, ETriggerEvent::Started, this, &ASBPlayerCharacter::OnSprint);
	PEI->BindAction(InputActions->SprintAction, ETriggerEvent::Completed, this, &ASBPlayerCharacter::OnSprint);

}

