
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBVehicleDisplayWidget.generated.h"

USTRUCT(BlueprintType)
struct FDisplayState
{
	GENERATED_BODY()

	float Speed;
	int32 Gear;
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
	class UTextBlock* CurrentGear;
	
};
