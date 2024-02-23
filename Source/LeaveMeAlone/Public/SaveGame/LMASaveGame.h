// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LMASaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ULMASaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	ULMASaveGame();

	// Maximum points.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float MaxScore;
};
