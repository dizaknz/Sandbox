#include "Vehicles/SBOffroadVehicle.h"
#include "Vehicles/VehicleActionData.h"

#include "ChaosVehicleMovementComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"


ASBOffroadVehicle::ASBOffroadVehicle()
{
	// set defaults for 4WD offroad vehicle
	// TODO: create custom offroad movement component
	//       Super(ObjectInitializer.SetDefaultSubobjectClass<UOffroadMovement>(VehicleComponentName))
	GetVehicleMovementComponent()->Mass = 1000;

	// TODO
	// GetVehicleMovementComponent()->TorqueControl

}

void ASBOffroadVehicle::OnThrottle(const FInputActionValue& Value)
{
	check(Controller);

	float Throttle = Value.Get<float>();
	GetVehicleMovementComponent()->SetThrottleInput(Throttle);
}

void ASBOffroadVehicle::OnBrake(const FInputActionValue& Value)
{
	check(Controller);

	float Brake = Value.Get<float>();
	GetVehicleMovementComponent()->SetBrakeInput(Brake);
}

void ASBOffroadVehicle::OnHandbrake(const FInputActionValue& Value)
{
	check(Controller);

	bool bHandbrake = Value.Get<bool>();
	GetVehicleMovementComponent()->SetHandbrakeInput(bHandbrake);
}

void ASBOffroadVehicle::LookAround(const FInputActionValue& Value)
{
	check(Controller);

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

void ASBOffroadVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(VehicleInputMapping);
	check(VehicleActions);
	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(VehicleInputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(VehicleActions->ThrottleAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnThrottle);
	PEI->BindAction(VehicleActions->BrakeAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnBrake);
	PEI->BindAction(VehicleActions->HandbrakeAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::OnHandbrake);
	PEI->BindAction(VehicleActions->LookAroundAction, ETriggerEvent::Triggered, this, &ASBOffroadVehicle::LookAround);
}

void ASBOffroadVehicle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// TODO: apply BP overrides during construction of vehicle
}
