// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LMAInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ULMAInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// Saving the result between levels.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveLavel")
	int32 Score = 0;
};
