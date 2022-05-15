#include "SBPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	// character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	// disable if not needed
	PrimaryActorTick.bCanEverTick = true;

	SprintSpeed = 1200.0f;
	WalkSpeed = 600.0f;
}

void ASBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBPlayerCharacter::MoveForward(float AxisValue)
{
	if ((Controller == nullptr) || (AxisValue == -0.0f))
	{
		// no movement allowed
		return;
	}

	// forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void ASBPlayerCharacter::MoveRight(float AxisValue)
{
	if ((Controller == nullptr) || (AxisValue == -0.0f))
	{
		// no movement allowed
		return;
	}
	// right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, AxisValue);
}

void ASBPlayerCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ASBPlayerCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASBPlayerCharacter::BeginCrouch()
{
	Crouch();
}

void ASBPlayerCharacter::EndCrouch()
{
	UnCrouch();
}

void ASBPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASBPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASBPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASBPlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASBPlayerCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASBPlayerCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASBPlayerCharacter::EndSprint);

}

