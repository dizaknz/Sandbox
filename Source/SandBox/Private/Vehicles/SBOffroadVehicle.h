#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "SBVehicleDisplayWidget.h"
#include "SBOffroadVehicle.generated.h"

struct FInputActionValue;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDisplayStateChange, const FDisplayState&, DisplayState);

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
	void Steer(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void OnInputChange(const FInputActionValue& Value);

private:
	float GetCurrentSpeedKPH();
	void UpdateDisplayState();

	UFUNCTION()
	void OnDisplayStateChange(const FDisplayState& DisplayState);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle Input")
	TObjectPtr<class UInputMappingContext> VehicleInputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle Input")
	TObjectPtr<class UVehicleActionData> VehicleActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Offroad Vehicle")
	float MaxOffroadSpeedKPH;

	UPROPERTY(VisibleAnywhere, Category = "Display")
	TObjectPtr<class UWidgetComponent> DisplayWidgetComponent;

	FOnDisplayStateChange DisplayStateChange;

private:
	// vehicle state
	float CurrentSpeed = -1.0f;
	int32 CurrentGear  = -1;
	
};
