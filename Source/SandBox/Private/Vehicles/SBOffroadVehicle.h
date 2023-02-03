#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "SBOffroadVehicle.generated.h"

struct FInputActionValue;

/**
 * Offroad Vehicle
 */
UCLASS()
class ASBOffroadVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ASBOffroadVehicle();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	void OnThrottle(const FInputActionValue& Value);
	void OnBrake(const FInputActionValue& Value);
	void OnHandbrake(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle Input")
	class UInputMappingContext* VehicleInputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle Input")
	class UVehicleActionData* VehicleActions;
	
};
