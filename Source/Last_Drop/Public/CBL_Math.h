// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CBL_Math.generated.h"

/**
 * 
 */
UCLASS()
class LAST_DROP_API UCBL_Math : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintPure, Category = "Create_Game")
	static FString GenerateVoidJumperCode();

	UFUNCTION(BlueprintCallable, Category = "Create_Game")
	static FString CreateUniqueRoomCode();

	UFUNCTION(BlueprintCallable, Category = "Create_Game")
	static void DestroyRoom(const FString& RoomCode);
};
