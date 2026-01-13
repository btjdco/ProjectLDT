// Copyright Epic Games, Inc. All Rights Reserved.

#include "Last_Drop.h"

#include "Modules/ModuleManager.h"
#include "MoviePlayer.h"
#include "SStartupLoadingScreen.h"

class FProjectNameGameModule : public FDefaultGameModuleImpl
{
public:
    virtual void StartupModule() override
    {
        // MoviePlayer가 활성화되어 있는 환경에서만S
        if (IsMoviePlayerEnabled())
        {
            FLoadingScreenAttributes Attrs;
            Attrs.bAutoCompleteWhenLoadingCompletes = true; // 첫 맵 로딩 끝나면 자동 종료
            Attrs.bWaitForManualStop = false;               // 수동 Stop 필요 없음
            Attrs.MinimumLoadingScreenDisplayTime = 5.0f;   // 너무 깜빡이는 것 방지 (선택)

            // Slate 위젯 지정
            Attrs.WidgetLoadingScreen = SNew(SStartupLoadingScreen);

            GetMoviePlayer()->SetupLoadingScreen(Attrs);

            // 중요: 부팅 직후 바로 표시 시작
            GetMoviePlayer()->PlayMovie();
        }
    }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FProjectNameGameModule, Last_Drop, "Last_Drop");