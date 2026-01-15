// Copyright Epic Games, Inc. All Rights Reserved.

#include "Last_Drop.h"
#include "UI/FLoadingScreenService.h"

class FProjectNameGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		// 게임 시작 직후 Entry 로딩 등록
		FLoadingScreenService::Show(ELoadingScreenType::Entry);
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FProjectNameGameModule, Last_Drop, "Last_Drop");