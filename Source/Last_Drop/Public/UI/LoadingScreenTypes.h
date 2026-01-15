#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ELoadingScreenType : uint8
{
	Entry        UMETA(DisplayName = "Entry"),
	EntryToMain  UMETA(DisplayName = "EntryToMain"),
	Master       UMETA(DisplayName = "Master"),
};

UENUM(BlueprintType)
enum class ETravelMode : uint8
{
	Auto           UMETA(DisplayName = "Auto"),
	Standalone     UMETA(DisplayName = "Standalone"),
	ListenHost     UMETA(DisplayName = "ListenHost"),
	ClientToServer UMETA(DisplayName = "ClientToServer"),
};