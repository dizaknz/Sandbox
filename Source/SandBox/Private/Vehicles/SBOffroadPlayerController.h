// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SBOffroadPlayerController.generated.h"

/**
 *  Offroad vehicle player controller 
 */
UCLASS(abstract)
class ASBOffroadPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnVehicleDisplayStateChange(const FDisplayState& DisplayState);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<class UInputMappingContext> VehicleInputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Display")
	TSubclassOf<class USBVehicleDisplayWidget> VehicleDisplayUIClass;

	UPROPERTY(VisibleAnywhere, Category = "Vehicle|Display")
	TObjectPtr<class USBVehicleDisplayWidget> VehicleDisplayUI;

protected:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

};
