// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Perception/AISightTargetInterface.h"
#include "Perception/AIPerceptionSystem.h"
#include "SBTeamUtils.generated.h"

/**
 * 
 */
UCLASS()
class USBTeamUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UAISense_Sight::EVisibilityResult CanActorBeSeenFrom(
		const AActor* Actor,
		const FCanBeSeenFromContext& Context,
		FVector& OutSeenLocation,
		int32& OutChecksPerformed,
		int32& OutAsyncCheckRequested,
		float& OutSightStrength,
		int32* UserData = nullptr,
		const FOnPendingVisibilityQueryProcessedDelegate* Delegate = nullptr);
};
