// Copyright Epic Games, Inc. All Rights Reserved.


#include "SBOffroadPlayerController.h"
#include "SBOffroadVehicle.h"
#include "SBVehicleDisplayWidget.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void ASBOffroadPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(VehicleInputMapping, 0);
	}

	VehicleDisplayUI = CreateWidget<USBVehicleDisplayWidget>(this, VehicleDisplayUIClass);
	check(VehicleDisplayUI);
	VehicleDisplayUI->AddToViewport();
}

void ASBOffroadPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// listen for vehicle state changes for UI
	TObjectPtr<ASBOffroadVehicle> OffroadVehiclePawn = Cast<ASBOffroadVehicle>(InPawn);
	if (IsValid(OffroadVehiclePawn))
	{
		// bind to display widget update
		OffroadVehiclePawn->DisplayStateChange.AddDynamic(this, &ASBOffroadPlayerController::OnVehicleDisplayStateChange);
	}


}

void ASBOffroadPlayerController::OnVehicleDisplayStateChange(const FDisplayState& DisplayState)
{
	VehicleDisplayUI->Update(DisplayState);
}


