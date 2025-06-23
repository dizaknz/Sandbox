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

ASBOffroadVehicle::ASBOffroadVehicle()
{
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -90.0f),
		FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Offroad Spring Arm"));
	SpringArm->SetupAttachment(GetMesh());

	// TODO: set in-car view, external-view
	SpringArm->TargetArmLength = 450;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 150.0f);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Offroad View Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	// TODO: create custom offroad movement component
	//       Super(ObjectInitializer.SetDefaultSubobjectClass<UOffroadMovement>(VehicleComponentName))
	// UWheeledVehicleMovementComponent4W
	// Configure the car mesh

	// get the Chaos Wheeled movement component
	OffroadMovementComponent = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());

	// Set sane defaults before BP overwrites
	// chassis
	OffroadMovementComponent->ChassisHeight = 160.0f;
	OffroadMovementComponent->ChassisWidth = 200.0f;
	OffroadMovementComponent->DragCoefficient = 0.1f;
	OffroadMovementComponent->DownforceCoefficient = 0.1f;
	OffroadMovementComponent->Mass = 1500;
	OffroadMovementComponent->CenterOfMassOverride = FVector(0.0f, 0.0f, 75.0f);
	OffroadMovementComponent->bEnableCenterOfMassOverride = true;

	// wheels setup in BP
	OffroadMovementComponent->bLegacyWheelFrictionPosition = true;
	OffroadMovementComponent->WheelSetups.SetNum(4);


	// engine
	OffroadMovementComponent->EngineSetup.MaxTorque = 600.0f;
	OffroadMovementComponent->EngineSetup.MaxRPM = 5000.0f;
	OffroadMovementComponent->EngineSetup.EngineIdleRPM = 1200.0f;
	OffroadMovementComponent->EngineSetup.EngineBrakeEffect = 0.05f;
	OffroadMovementComponent->EngineSetup.EngineRevUpMOI = 5.0f;
	OffroadMovementComponent->EngineSetup.EngineRevDownRate = 600.0f;

	// differential
	OffroadMovementComponent->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
	OffroadMovementComponent->DifferentialSetup.FrontRearSplit = 0.5f;

	// steering
	OffroadMovementComponent->SteeringSetup.SteeringType = ESteeringType::AngleRatio;
	OffroadMovementComponent->SteeringSetup.AngleRatio = 0.7f;

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
	
}

void ASBOffroadVehicle::OnThrottle(const FInputActionValue& Value)
{
	check(Controller);

	float Throttle = Value.Get<float>();
	UE_LOG(LogVehicles, Display, TEXT("On throttle: %f"), Value.GetMagnitude());

	OffroadMovementComponent->SetThrottleInput(Throttle);
	UpdateDisplayState();

}

void ASBOffroadVehicle::OnBrake(const FInputActionValue& Value)
{
	check(Controller);
	
	float fBrake = Value.Get<float>();
    float fThrottle = OffroadMovementComponent->GetThrottleInput();
	UE_LOG(LogVehicles, Display, TEXT("On brake: %f"), Value.GetMagnitude());

	if (fBrake != 0 && fThrottle > 0)
	{
		// reset throttle
		OffroadMovementComponent->DecreaseThrottleInput(fThrottle);
	}
	OffroadMovementComponent->SetBrakeInput(fBrake);
	UpdateDisplayState();

}

void ASBOffroadVehicle::OnHandbrake(const FInputActionValue& Value)
{
	check(Controller);

	bool bHandbrake = Value.Get<bool>();
	UE_LOG(LogVehicles, Display, TEXT("On handbrake: %s"), bHandbrake ? TEXT("true") : TEXT("false"));
	OffroadMovementComponent->SetHandbrakeInput(bHandbrake);
	UpdateDisplayState();

}

void ASBOffroadVehicle::Steer(const FInputActionValue& Value)
{
	check(Controller);

	float SteerValue = Value.Get<float>();
	UE_LOG(LogVehicles, Display, TEXT("Steering: %f"), SteerValue);
	OffroadMovementComponent->SetSteeringInput(SteerValue);
}

void ASBOffroadVehicle::LookAround(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	FRotator ViewRotator;
	ViewRotator.Yaw = LookValue.X;
	//ViewRotator.Pitch = LookValue.Y;

	check(SpringArm);
	SpringArm->AddRelativeRotation(ViewRotator);

}

void ASBOffroadVehicle::ResetVehicle(const FInputActionValue& Value)
{
	FVector ResetLocation = GetActorLocation();
	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Roll = 0.0f;
	
	// teleport to reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);
	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	UpdateDisplayState();

}

void ASBOffroadVehicle::UpdateDisplayState()
{
	float Speed = OffroadMovementComponent->GetForwardSpeedMPH();
	int32 Gear = OffroadMovementComponent->GetCurrentGear();

	FDisplayState Display;
	Display.bUseKph = bSpeedInKph;
	Display.Speed = Speed;
	Display.Gear = Gear;
	Display.RPM = int32(OffroadMovementComponent->GetEngineRotationSpeed());

	DisplayStateChange.Broadcast(Display);
}

void ASBOffroadVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	checkf(VehicleActions, TEXT("Setup error: provide vehicle action data asset"));
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// throttle
	PEI->BindAction(VehicleActions->ThrottleAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnThrottle);
	PEI->BindAction(VehicleActions->ThrottleAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::OnThrottle);

	// brake
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnBrake);
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::OnBrake);

	// handbrake
	PEI->BindAction(VehicleActions->HandbrakeAction, ETriggerEvent::Started, this, &ASBOffroadVehicle::OnHandbrake);
	PEI->BindAction(VehicleActions->HandbrakeAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::OnHandbrake);

	// steer
	PEI->BindAction(VehicleActions->SteeringAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::Steer);
	PEI->BindAction(VehicleActions->SteeringAction, ETriggerEvent::Completed, this, &ASBOffroadVehicle::Steer);

	// look around
	PEI->BindAction(VehicleActions->LookAroundAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::LookAround);

	// reset vehicle
	PEI->BindAction(VehicleActions->ResetVehicleAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::ResetVehicle);
}

void ASBOffroadVehicle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void ASBOffroadVehicle::BeginPlay()
{
	Super::BeginPlay();

}
