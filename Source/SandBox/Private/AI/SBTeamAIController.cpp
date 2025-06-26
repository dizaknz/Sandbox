#include "AI/SBTeamAIController.h"
#include "AI/SBTeamAIConfig.h"
#include "AI/SBEnemyCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Engine/World.h"

ASBTeamAIController::ASBTeamAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);
}

void ASBTeamAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    if (auto* Enemy = Cast<ASBEnemyCharacter>(InPawn))
    {
        SetGenericTeamId(Enemy->GetGenericTeamId());
        SetupBehaviorTrees(Enemy->AIConfig);
        SetupAIPerception(Enemy->AIConfig);
    }

    // TODO: enemy vehicle
}

ETeamAttitude::Type ASBTeamAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (const auto* TeamAgent = Cast<IGenericTeamAgentInterface>(&Other))
    {
        FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
    
        if (OtherTeamID.GetId() == 255)
        {
            // no team is the default
            return ETeamAttitude::Neutral;
        }
        else if (OtherTeamID.GetId() == GetGenericTeamId())
        {
            return ETeamAttitude::Friendly;
        }
        else if (OtherTeamID.GetId() != GetGenericTeamId())
        {
            return ETeamAttitude::Hostile;
        }
    }
    return ETeamAttitude::Neutral;
    
}
    
ETeamAttitude::Type ASBTeamAIController::GetTeamAttitudeRelationship(const AActor* Other) const
{
    return GetTeamAttitudeTowards(*Other);
}
    
void ASBTeamAIController::SetupBehaviorTrees(const TObjectPtr<USBTeamAIConfig> AIData)
{
    if (IsValid(AIData) && IsValid(AIData->BaseBehavior))
    {
        RunBehaviorTree(AIData->BaseBehavior);
    
        // dynamic subtrees
        if (auto* BehaviorTreeComp = FindComponentByClass<UBehaviorTreeComponent>())
        {
            for (const auto& Entry : AIData->DynamicBehaviors)
            {
                if (IsValid(Entry.BehaviorTree))
                {
                    BehaviorTreeComp->SetDynamicSubtree(Entry.StateTag, Entry.BehaviorTree);
                }
            }
        }
    }
}
    
void ASBTeamAIController::SetupAIPerception(const TObjectPtr<USBTeamAIConfig> AIData)
{
    if (!IsValid(AIPerceptionComponent) || !IsValid(AIData))
    {
        return;
    }
    if (AIData->SensesConfig.Num() > 0)
    {
        for (UAISenseConfig* SenseConfig : AIData->SensesConfig)
        {
            if (IsValid(SenseConfig))
            {
                AIPerceptionComponent->ConfigureSense(*SenseConfig);
            }
        }

        if (auto* AIPerceptionSys = UAIPerceptionSystem::GetCurrent(GetWorld()))
        {
            AIPerceptionSys->UpdateListener(*AIPerceptionComponent);
        }
    }
    if (AIData->DominantSense)
    {
        AIPerceptionComponent->SetDominantSense(AIData->DominantSense);
    }
}
