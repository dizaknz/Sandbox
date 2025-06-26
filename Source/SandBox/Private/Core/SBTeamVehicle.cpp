#include "SBTeamVehicle.h"
#include "Core/SBTeamUtils.h"

ASBTeamVehicle::ASBTeamVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASBTeamVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FGenericTeamId ASBTeamVehicle::GetGenericTeamId() const
{
	return TeamID;
}
	
bool ASBTeamVehicle::CanBeSeenOverride_Implementation()
{
	return true;
}
	
UAISense_Sight::EVisibilityResult ASBTeamVehicle::CanBeSeenFrom(
	const FCanBeSeenFromContext& Context,
	FVector& OutSeenLocation,
	int32& OutChecksPerformed,
	int32& OutAsyncCheckRequested,
	float& OutSightStrength,
	int32* UserData,
	const FOnPendingVisibilityQueryProcessedDelegate* Delegate)		
{
	if (!CanBeSeenOverride())
	{
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	return USBTeamUtils::CanActorBeSeenFrom(
		this,
		Context,
		OutSeenLocation,
		OutChecksPerformed,
		OutAsyncCheckRequested,
		OutSightStrength,
		UserData,
		Delegate);	
}