// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/CoreStyle.h"
#include "Animation/CurveSequence.h" // 애니메이션 헤더 추가

class SStartupLoadingScreen : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SStartupLoadingScreen) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    // 이미지가 나타나는 투명도(Opacity)를 계산하는 함수
    FSlateColor GetLogoColorAndOpacity() const;

private:
    const FString LogoPath = TEXT("/Game/NewProject/Rendering/UMG/T_Splash_v2.T_Splash_v2");

    FVector2D AnimationTiming = FVector2D(0.0f, 3.0f);
    FVector2D ImageSize = FVector2D(300.0f, 300.0f);

    TSharedPtr<FSlateBrush> LogoBrush; // 메모리 관리용 스마트 포인터로 브러시
    FCurveSequence FadeInCurve; // 애니메이션 관련 변수
    USoundBase* StartupSound; // 사운드 에셋을 담을 변수

};