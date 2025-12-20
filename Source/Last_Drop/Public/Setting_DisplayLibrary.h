// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Setting_DisplayLibrary.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDisplayInfo {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	int32 NativeWidth;

	UPROPERTY(BlueprintReadOnly)
	int32 NativeHeight;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint MaxResolution;

	//Default Constructor
	FDisplayInfo()
	{
		Name = FString("");
		NativeWidth = 0;
		NativeHeight = 0;
		MaxResolution = FIntPoint(ForceInitToZero);
	}
	FDisplayInfo
	(
		const FString& InName, int32 InNativeWidth, 
		int32 InNativeHeight,const FIntPoint& InMaxResolution
	) 
		: Name(InName),
		NativeWidth(InNativeWidth),
		NativeHeight(InNativeHeight),
		MaxResolution(InMaxResolution)
	{

	}
};

UCLASS()
class LAST_DROP_API USetting_DisplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "L_Setting_Display")
	static int GetDisplayCount();

	UFUNCTION(BlueprintPure, Category = "L_Setting_Display")
	static TArray<FDisplayInfo> GetAllDisplays();
	
	UFUNCTION(BlueprintCallable, Category = "L_Setting_Display")
	static void SetActiveDisplay(int32 NewDisplayIndex);
};
