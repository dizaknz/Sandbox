#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SBTeamAIConfig.generated.h"

class UAISenseConfig;
class UAISense;
	
USTRUCT(BlueprintType)
struct FTaggedBehaviorTree
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StateTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBehaviorTree* BehaviorTree = nullptr;
};

/**
 * Enemy AI Config
 */
UCLASS()
class USBEnemyConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Logic")
	UBehaviorTree* BaseBehavior = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Logic")
	TArray<FTaggedBehaviorTree> DynamicBehaviors;
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "AI Perception")
	TArray<TObjectPtr<UAISenseConfig>> SensesConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Perception")
	TSubclassOf<UAISense> DominantSense;
	
};
