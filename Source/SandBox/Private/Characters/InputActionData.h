#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "InputActionData.generated.h"

/**
 * Character input actions
 */
UCLASS()
class UInputActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SprintAction;

};
