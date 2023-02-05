#include "Vehicles/SBOffroadVehicle.h"
#include "Vehicles/VehicleActionData.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"


ASBOffroadVehicle::ASBOffroadVehicle()
{
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -90.0f),
		FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	// TODO: set in-car view, external-view
	SpringArm->TargetArmLength = 450;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 150.0f);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// set defaults for 4WD offroad vehicle
	// TODO: create custom offroad movement component
	//       Super(ObjectInitializer.SetDefaultSubobjectClass<UOffroadMovement>(VehicleComponentName))
	UChaosVehicleMovementComponent * OffroadMovementComponent = GetVehicleMovementComponent();
	OffroadMovementComponent->Mass = 1000;

	// TODO
	// AWDMovementComponent->TorqueControl

	// auto by default
	OffroadMovementComponent->SetUseAutomaticGears(true);

	OffroadMovementComponent->EnableSelfRighting(true);
	MaxOffroadSpeedKPH = 100;

}

void ASBOffroadVehicle::OnThrottle(const FInputActionValue& Value)
{
	check(Controller);

	float Throttle = Value.Get<float>();
	float SpeedKPH = GetCurrentSpeedKPH();

	UE_LOG(LogTemp, Log, 
		TEXT("Delta Throttle: %f Raw Throttle: %f Speed KPH: %f Gear: %d"), 
		Throttle,
		GetVehicleMovementComponent()->GetThrottleInput(),
		SpeedKPH,
		GetVehicleMovementComponent()->GetCurrentGear());

	GetVehicleMovementComponent()->SetThrottleInput(Throttle);
}

void ASBOffroadVehicle::OnBrake(const FInputActionValue& Value)
{
	check(Controller);
	
	float bBrake = Value.Get<float>();

	float Throttle = GetVehicleMovementComponent()->GetThrottleInput();

	if (bBrake && Throttle > 0)
	{
		// reset throttle
		GetVehicleMovementComponent()->DecreaseThrottleInput(Throttle);
	}
	UE_LOG(LogTemp, Log,
		TEXT("Brake: %f Speed KPH: %f Gear: %d"), 
		bBrake,
		GetCurrentSpeedKPH(),
		GetVehicleMovementComponent()->GetCurrentGear());
	GetVehicleMovementComponent()->SetBrakeInput(bBrake);
}

void ASBOffroadVehicle::OnHandbrake(const FInputActionValue& Value)
{
	check(Controller);

	bool bHandbrake = Value.Get<bool>();
	GetVehicleMovementComponent()->SetHandbrakeInput(bHandbrake);
}

void ASBOffroadVehicle::Steer(const FInputActionValue& Value)
{
	check(Controller);

	float SteerValue = Value.Get<float>();
	UE_LOG(LogTemp, Log, TEXT("Steering: %f"), SteerValue);
	GetVehicleMovementComponent()->SetSteeringInput(SteerValue);
}

void ASBOffroadVehicle::LookAround(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	FRotator ViewRotator;
	ViewRotator.Yaw = LookValue.X;
	ViewRotator.Pitch = LookValue.Y;

	SpringArm->AddRelativeRotation(ViewRotator);
}

float ASBOffroadVehicle::GetCurrentSpeedKPH()
{
	return GetVehicleMovementComponent()->GetForwardSpeedMPH() * 1.60934f;
}

void ASBOffroadVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(VehicleInputMapping, TEXT("Setup error: provide vehicle input mapping"));
	checkf(VehicleActions, TEXT("Setup error: provide vehicle action data asset"));
	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(VehicleInputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(VehicleActions->ThrottleAction, ETriggerEvent::Started, this, &ASBOffroadVehicle::OnThrottle);
	PEI->BindAction(VehicleActions->ThrottleAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::OnThrottle);
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Started, this, &ASBOffroadVehicle::OnBrake);
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::OnBrake);
	PEI->BindAction(VehicleActions->HandbrakeAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnHandbrake);
	PEI->BindAction(VehicleActions->SteeringAction, ETriggerEvent::Started, this, &ASBOffroadVehicle::Steer);
	PEI->BindAction(VehicleActions->SteeringAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::Steer);
	PEI->BindAction(VehicleActions->LookAroundAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::LookAround);
}

void ASBOffroadVehicle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}
