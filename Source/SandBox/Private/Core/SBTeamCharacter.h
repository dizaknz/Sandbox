#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISightTargetInterface.h"
#include "Perception/AIPerceptionSystem.h"

#include "SBTeamCharacter.generated.h"

// https://dev.epicgames.com/community/learning/tutorials/z0G7/how-to-setup-your-ai-the-right-way-unreal-engine

/**
 * Team Character that implements the team agent interface
 */
UCLASS(Abstract)
class ASBTeamCharacter : public ACharacter, public IGenericTeamAgentInterface, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	ASBTeamCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	 
	virtual FGenericTeamId GetGenericTeamId() const override;
	 
	UFUNCTION(BlueprintNativeEvent)
	bool CanBeSeenOverride();
	 
	bool CanBeSeenOverride_Implementation();
	 
	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(
		const FCanBeSeenFromContext& Context,
		FVector& OutSeenLocation,
		int32& OutNumberChecksPerformed,
		int32& OutNumberOfAsyncCheckRequested,
		float& OutSightStrength,
		int32* UserData = nullptr,
		const FOnPendingVisibilityQueryProcessedDelegate* Delegate = nullptr) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	FGenericTeamId TeamID = FGenericTeamId(255);

};
