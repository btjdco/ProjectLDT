// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GPUInfoLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LAST_DROP_API UGPUInfoLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    /** User GPU name (e.g. "NVIDIA GeForce RTX 3080") */
    UFUNCTION(BlueprintPure, Category = "GPU")
    static FString GetGPUName();
};
