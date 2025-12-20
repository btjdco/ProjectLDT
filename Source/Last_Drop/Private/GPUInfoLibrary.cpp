// Fill out your copyright notice in the Description page of Project Settings.


#include "GPUInfoLibrary.h"
#include "RHI.h"                // GRHIAdapterName
#include "Misc/App.h"          // FApp::GetGraphicsRHI
#include "RHIResources.h"
#include "GameFramework/GameUserSettings.h"

FString UGPUInfoLibrary::GetGPUName()
{
	return GRHIAdapterName;
}



