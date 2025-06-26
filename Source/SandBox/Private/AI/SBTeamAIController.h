#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SBTeamAIController.generated.h"

class USBTeamAIConfig;

/**
 * Enemy Controller
 */
UCLASS()
class ASBTeamAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASBTeamAIController();

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	 
private:
	void SetupBehaviorTrees(const TObjectPtr<USBTeamAIConfig> AIData);
	void SetupAIPerception(const TObjectPtr<USBTeamAIConfig> AIData);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	 
	UFUNCTION(BlueprintCallable, Category = "Team")
	ETeamAttitude::Type GetTeamAttitudeRelationship(const AActor* Other) const;

};
