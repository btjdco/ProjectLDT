// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/FLoadingScreenService.h"

#include "MoviePlayer.h"
#include "Widgets/SWidget.h"

// 너가 만든 Slate 위젯들 include
#include "UI/SEntryLoadingWidget.h"
#include "UI/SMainLoadingWidget.h"
// #include "SMasterLoadingWidget.h"

TSharedRef<SWidget> FLoadingScreenService::CreateWidget(ELoadingScreenType Type)
{
	switch (Type)
	{
	case ELoadingScreenType::Entry:
		return SNew(SEntryLoadingWidget);

	case ELoadingScreenType::EntryToMain:
		return SNew(SMainLoadingWidget);

		// case ELoadingScreenType::Master:
		// 	return SNew(SMasterLoadingWidget);

	default:
		return SNew(SEntryLoadingWidget);
	}
}

void FLoadingScreenService::Show(ELoadingScreenType Type)
{
	if (!IsMoviePlayerEnabled())
	{
		// 에디터 PIE 등 특정 상황에서 MoviePlayer 비활성일 수 있음
		return;
	}

	FLoadingScreenAttributes Attr;
	Attr.bAutoCompleteWhenLoadingCompletes = true; // 로딩 끝나면 자동 내림(기본 추천)
	Attr.bWaitForManualStop = false;
	Attr.bAllowEngineTick = false; // 로딩 중 게임 틱 필요하면 true (보통 false)
	Attr.MinimumLoadingScreenDisplayTime = 3.0f;

	Attr.WidgetLoadingScreen = CreateWidget(Type);

	GetMoviePlayer()->SetupLoadingScreen(Attr);
}

void FLoadingScreenService::Hide()
{
	if (IsMoviePlayerEnabled())
	{
		GetMoviePlayer()->StopMovie();
	}
}
