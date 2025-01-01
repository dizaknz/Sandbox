#include "Vehicles/SBOffroadVehicle.h"
#include "Vehicles/VehicleActionData.h"
#include "Log.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <ChaosVehicleMovementComponent.h>
#include <InputMappingContext.h>
#include <EnhancedInputSubsystems.h>
#include <InputActionValue.h>
#include <EnhancedInputComponent.h>
#include <Components/WidgetComponent.h>

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

	DisplayWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DisplayWidget"));
	DisplayWidgetComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	// set defaults for 4WD offroad vehicle
	// TODO: create custom offroad movement component
	//       Super(ObjectInitializer.SetDefaultSubobjectClass<UOffroadMovement>(VehicleComponentName))
	// UWheeledVehicleMovementComponent4W
	UChaosVehicleMovementComponent * OffroadMovementComponent = GetVehicleMovementComponent();

	// Set sane defaults before BP overwrites:
	OffroadMovementComponent->Mass = 1500;
	OffroadMovementComponent->ChassisWidth = 200.0f;
	OffroadMovementComponent->ChassisHeight = 50.0f;
	OffroadMovementComponent->DragCoefficient = 2.0f;
	OffroadMovementComponent->DownforceCoefficient = 10.0f;
	OffroadMovementComponent->DragArea = 7.5f;
	OffroadMovementComponent->SleepThreshold = 10.0f;
	OffroadMovementComponent->SleepSlopeLimit = 0.866f;
	OffroadMovementComponent->DownforceCoefficient = 10.0f;

	// BP overwrites per vehicle SM:
	// bEnableCenterOfMassOverride
	// CenterOfMassOverride
	// InertiaTensorScale
	
	// TODO:
	// Arcade mode:
	// TorqueControl
	// TargetRotationControl
	// StabilizeControl
	//
	// Vehicle setup:
	// Aerofoils
	// Thrusters (rocket boost)
	// 
	// Player:
	// SetOverrideController - player pawn override

	// auto by default
	// TODO: support changing to manual
	OffroadMovementComponent->SetUseAutomaticGears(true);
	OffroadMovementComponent->EnableSelfRighting(true);
	OffroadMovementComponent->SetRequiresControllerForInputs(true);
	
	// default
	MaxOffroadSpeedKPH = 100;
	CurrentSpeed = 0;
	CurrentGear = 0;

	// bind to display widget update
	DisplayStateChange.BindDynamic(this, &ASBOffroadVehicle::OnDisplayStateChange);

}

void ASBOffroadVehicle::OnThrottle(const FInputActionValue& Value)
{
	check(Controller);

	float Throttle = Value.Get<float>();
	UE_LOG(LogVehicles, Verbose, TEXT("On throttle: %f"), Value.GetMagnitude());

	GetVehicleMovementComponent()->SetThrottleInput(Throttle);

}

void ASBOffroadVehicle::OnBrake(const FInputActionValue& Value)
{
	check(Controller);
	
	float fBrake = Value.Get<float>();
    float fThrottle = GetVehicleMovementComponent()->GetThrottleInput();
	UE_LOG(LogVehicles, Verbose, TEXT("On brake: %f"), Value.GetMagnitude());

	if (fBrake != 0 && fThrottle > 0)
	{
		// reset throttle
		GetVehicleMovementComponent()->DecreaseThrottleInput(fThrottle);
	}

	GetVehicleMovementComponent()->SetBrakeInput(fBrake);

}

void ASBOffroadVehicle::OnHandbrake(const FInputActionValue& Value)
{
	check(Controller);

	bool bHandbrake = Value.Get<bool>();
	UE_LOG(LogVehicles, Verbose, TEXT("On handbrake: %s"), bHandbrake ? TEXT("true") : TEXT("false"));
	GetVehicleMovementComponent()->SetHandbrakeInput(bHandbrake);
}

void ASBOffroadVehicle::Steer(const FInputActionValue& Value)
{
	check(Controller);

	float SteerValue = Value.Get<float>();
	UE_LOG(LogVehicles, Verbose, TEXT("Steering: %f"), SteerValue);
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

void ASBOffroadVehicle::OnInputChange(const FInputActionValue& Value)
{
	UpdateDisplayState();
}

float ASBOffroadVehicle::GetCurrentSpeedKPH()
{
	return GetVehicleMovementComponent()->GetForwardSpeedMPH() * 1.60934f;
}

void ASBOffroadVehicle::UpdateDisplayState()
{
	float SpeedKPH = GetCurrentSpeedKPH();
	int32 Gear = GetVehicleMovementComponent()->GetCurrentGear();

	if (SpeedKPH != CurrentSpeed || Gear != CurrentGear)
	{
		CurrentSpeed = SpeedKPH;
		CurrentGear = Gear;
		FDisplayState Display;
		Display.Speed = CurrentSpeed;
		Display.Gear = CurrentGear;
		DisplayStateChange.ExecuteIfBound(Display);
	}
}

void ASBOffroadVehicle::OnDisplayStateChange(const FDisplayState& DisplayState)
{
	USBVehicleDisplayWidget* DisplayWidget = Cast<USBVehicleDisplayWidget>(DisplayWidgetComponent->GetUserWidgetObject());
	if (!IsValid(DisplayWidget))
	{
		return;
	}
	DisplayWidget->Update(DisplayState);
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
	PEI->BindAction(VehicleActions->ThrottleAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnInputChange);
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Started, this, &ASBOffroadVehicle::OnBrake);
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::OnBrake);
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnInputChange);
	PEI->BindAction(VehicleActions->HandbrakeAction, ETriggerEvent::Started, this, &ASBOffroadVehicle::OnHandbrake);
	PEI->BindAction(VehicleActions->HandbrakeAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::OnHandbrake);
	PEI->BindAction(VehicleActions->HandbrakeAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnInputChange);
	PEI->BindAction(VehicleActions->SteeringAction, ETriggerEvent::Started, this, &ASBOffroadVehicle::Steer);
	PEI->BindAction(VehicleActions->SteeringAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::Steer);
	PEI->BindAction(VehicleActions->LookAroundAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::LookAround);
}

void ASBOffroadVehicle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}
