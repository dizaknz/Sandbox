#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "VehicleActionData.generated.h"

/**
 * Character input actions
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
	UInputAction* LookAroundAction;

};
