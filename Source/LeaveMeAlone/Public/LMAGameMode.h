// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/LMASaveGame.h"
#include "GameFramework/GameModeBase.h"
#include "LMAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ALMAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	// Name of the save slot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
	FString SaveSlotName;

	// Index of the save slot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
	int32 UserIndex;

	UFUNCTION(BlueprintCallable)
	int32 LoadGameMaxScore() const;

	UFUNCTION(BlueprintCallable)
	void SaveGameMaxScore(int32 Score) const;

	UFUNCTION(BlueprintCallable)
	void ResetGameMaxScore() ;

private:
	
	ALMAGameMode();

	bool ResetMaxScore = false;
};
