// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LMAGameMode.h"
#include "SaveGame/LMASaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LMADefaultCharacter.h"
#include "Player/LMAPlayerController.h"

ALMAGameMode::ALMAGameMode() {

	DefaultPawnClass = ALMADefaultCharacter::StaticClass();
	PlayerControllerClass = ALMAPlayerController::StaticClass();

	SaveSlotName = TEXT("MaxScore");
	UserIndex = 0;
}

int32 ALMAGameMode::LoadGameMaxScore() const {

	ULMASaveGame* LoadGameScore = Cast<ULMASaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	
	if (IsValid(LoadGameScore)) {
	
		return LoadGameScore->MaxScore;
	}

	return 0;
}

void ALMAGameMode::SaveGameMaxScore(int32 Score) const {

	ULMASaveGame* SaveGameScore = Cast<ULMASaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

	if (IsValid(SaveGameScore)) {

		if (Score > SaveGameScore->MaxScore || ResetMaxScore) {
		
			SaveGameScore->MaxScore = Score;
			UGameplayStatics::SaveGameToSlot(SaveGameScore, SaveSlotName, UserIndex);
		}
	}
	else {

		ULMASaveGame* SaveGameInstance = Cast<ULMASaveGame>(UGameplayStatics::CreateSaveGameObject(ULMASaveGame::StaticClass()));
		SaveGameInstance->MaxScore = Score;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);
	}
}

void ALMAGameMode::ResetGameMaxScore()  {

	ResetMaxScore = true;
	SaveGameMaxScore(0);
	ResetMaxScore = false;
}
