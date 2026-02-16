// ZombieAIController.h
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

UCLASS()
class LYRAGAME_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AZombieAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;


protected:
};