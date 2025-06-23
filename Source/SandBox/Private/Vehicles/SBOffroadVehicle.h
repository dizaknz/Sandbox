#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "SBVehicleDisplayWidget.h"
#include "Core/SBTeamCharacter.h"

#include "SBOffroadVehicle.generated.h"

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisplayStateChange, const FDisplayState&, DisplayState);

/**
 * Offroad Vehicle
 */
UCLASS(Abstract)
class ASBOffroadVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ASBOffroadVehicle();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	/** Read only */
	FORCEINLINE const TObjectPtr<UChaosWheeledVehicleMovementComponent>& GetOffroadMovementComponent() const
	{ 
		return OffroadMovementComponent;
	}

protected:
	void OnThrottle(const FInputActionValue& Value);
	void OnBrake(const FInputActionValue& Value);
	void OnHandbrake(const FInputActionValue& Value);
	void Steer(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void ResetVehicle(const FInputActionValue& Value);

private:
	void UpdateDisplayState();

public:
	UPROPERTY(BlueprintAssignable, Category = "Display State")
	FOnDisplayStateChange DisplayStateChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	bool bSpeedInKph = true;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<class UVehicleActionData> VehicleActions;

	TObjectPtr<UChaosWheeledVehicleMovementComponent> OffroadMovementComponent;

};
