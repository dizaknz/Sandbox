#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "VehicleActionData.generated.h"

/**
 * Vehcile input actions
 */
UCLASS()
class UVehicleActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ThrottleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* BrakeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* HandbrakeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SteeringAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* LookAroundAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ResetVehicleAction;

};
