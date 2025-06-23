#include "SBTeamCharacter.h"

ASBTeamCharacter::ASBTeamCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASBTeamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FGenericTeamId ASBTeamCharacter::GetGenericTeamId() const
{
	return TeamID;
}
	
bool ASBTeamCharacter::CanBeSeenOverride_Implementation()
{
	return true;
}
	
UAISense_Sight::EVisibilityResult ASBTeamCharacter::CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	if (!CanBeSeenOverride())
	{
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	
	FHitResult HitResult;
	const auto& TargetLocation = GetActorLocation();
	const auto& DefaultSightCollisionChannel = GET_AI_CONFIG_VAR(DefaultSightCollisionChannel);
	const FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(AILineOfSight), true, Context.IgnoreActor);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Context.ObserverLocation, TargetLocation, DefaultSightCollisionChannel, QueryParams, FCollisionResponseParams::DefaultResponseParam);
	
	++OutNumberOfLoSChecksPerformed;
	
	const auto IsTraceConsideredVisible = [](const FHitResult& HitResult, const AActor* TargetActor)
		{
			const AActor* HitResultActor = HitResult.HitObjectHandle.FetchActor();
			return (HitResultActor ? HitResultActor->IsOwnedBy(TargetActor) : false);
		};
	
	if (!bHit || IsTraceConsideredVisible(HitResult, this))
	{
		OutSeenLocation = TargetLocation;
		return UAISense_Sight::EVisibilityResult::Visible;
	}
	
	return UAISense_Sight::EVisibilityResult::NotVisible;
}