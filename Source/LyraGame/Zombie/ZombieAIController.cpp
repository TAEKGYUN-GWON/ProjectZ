// ZombieAIController.cpp
#include "ZombieAIController.h"
#include "GameFramework/PlayerState.h"
#include "Teams/LyraTeamAgentInterface.h"

AZombieAIController::AZombieAIController()
{
	
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}