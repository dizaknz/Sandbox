
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBVehicleDisplayWidget.generated.h"

USTRUCT(BlueprintType)
struct FDisplayState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Display")
	bool bUseKph = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Display")
	int32 Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Display")
	int32 Gear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Display")
	int32 RPM;
};

/**
 * Live Info Display
 */
UCLASS()
class USBVehicleDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Update(const FDisplayState& State);

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentSpeed;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SpeedUnit;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentGear;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentRPM;
	
};
