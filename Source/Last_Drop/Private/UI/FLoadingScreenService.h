// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LoadingScreenTypes.h"

class FLoadingScreenService
{
public:
	static void Show(ELoadingScreenType Type);
	static void Hide(); // (필요하면)S

private:
	static TSharedRef<SWidget> CreateWidget(ELoadingScreenType Type);
};