// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/Setting_DisplayLibrary.h"
#include "GenericPlatform/GenericApplication.h"

#include "Framework/Application/SlateApplication.h"
#include "GenericPlatform/GenericWindow.h"

#include "Engine.h"
#include "Widgets/SWindow.h"

int USetting_DisplayLibrary::GetDisplayCount()
{
	FDisplayMetrics LocalDisplays;
	FDisplayMetrics::RebuildDisplayMetrics(LocalDisplays);

	return LocalDisplays.MonitorInfo.Num();
}

TArray<FDisplayInfo> USetting_DisplayLibrary::GetAllDisplays()
{
	TArray<FDisplayInfo> LocalAllDisplays;

	FDisplayMetrics LocalDisplay;
	FDisplayMetrics::RebuildDisplayMetrics(LocalDisplay);
	
	for (const FMonitorInfo& Monitor : LocalDisplay.MonitorInfo)
	{
		LocalAllDisplays.
			Add(FDisplayInfo
			(
				Monitor.Name, Monitor.NativeWidth, Monitor.NativeHeight,Monitor.MaxResolution
			)
		);
	}

	return LocalAllDisplays;
}

void USetting_DisplayLibrary::SetActiveDisplay(int32 NewDisplayIndex)
{
	FDisplayMetrics LocalDisplays;
	FDisplayMetrics::RebuildDisplayMetrics(LocalDisplays);

	if (NewDisplayIndex > LocalDisplays.MonitorInfo.Num())
	{
		//Non existing display
		return;
	}

	const FMonitorInfo TargetMonitor = LocalDisplays.MonitorInfo[NewDisplayIndex];
	FVector2D WindowPosition(static_cast<float>(TargetMonitor.WorkArea.Left), static_cast<float>(TargetMonitor.WorkArea.Top));


	if (GEngine && GEngine->GameViewport)
	{
		TSharedPtr<SWindow> GWindow = GEngine->GameViewport->GetWindow();
		GWindow->MoveWindowTo(WindowPosition);
	}

	return;
}


