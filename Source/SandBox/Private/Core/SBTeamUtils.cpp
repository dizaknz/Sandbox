#include "Core/SBTeamUtils.h"

UAISense_Sight::EVisibilityResult USBTeamUtils::CanActorBeSeenFrom(
    const AActor* Actor,
    const FCanBeSeenFromContext& Context,
    FVector& OutSeenLocation,
    int32& OutChecksPerformed,
    int32& OutAsyncCheckRequested,
    float& OutSightStrength,
    int32* UserData,
    const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	FHitResult HitResult;
	const auto& TargetLocation = Actor->GetActorLocation();
	const auto& DefaultSightCollisionChannel = GET_AI_CONFIG_VAR(DefaultSightCollisionChannel);
	const FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(AILineOfSight), true, Context.IgnoreActor);
	const bool bHit = Actor->GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Context.ObserverLocation,
        TargetLocation,
        DefaultSightCollisionChannel,
        QueryParams,
        FCollisionResponseParams::DefaultResponseParam);
	
	++OutChecksPerformed;
	
	const auto IsTraceConsideredVisible = [](const FHitResult& HitResult, const AActor* TargetActor)
    {
        const AActor* HitResultActor = HitResult.HitObjectHandle.FetchActor();
        return (HitResultActor ? HitResultActor->IsOwnedBy(TargetActor) : false);
    };
	
	if (!bHit || IsTraceConsideredVisible(HitResult, Actor))
	{
		OutSeenLocation = TargetLocation;
		return UAISense_Sight::EVisibilityResult::Visible;
	}
	
	return UAISense_Sight::EVisibilityResult::NotVisible;
}
