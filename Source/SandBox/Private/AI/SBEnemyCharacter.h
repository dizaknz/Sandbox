#pragma once

#include "CoreMinimal.h"
#include "Core/SBTeamCharacter.h"
#include "AI/SBTeamAIConfig.h"
#include "SBEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ASBEnemyCharacter : public ASBTeamCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (DisplayPriority = "1", AllowPrivateAccess = "true"))
	TObjectPtr<USBTeamAIConfig> AIConfig = nullptr;
	
};
