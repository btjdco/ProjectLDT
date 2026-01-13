// Fill out your copyright notice in the Description page of Project Settings.


#include "SStartupLoadingScreen.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SThrobber.h" // 점 애니메이션
#include "Widgets/Images/SImage.h" // SImage 사용을 위해 추가

#include "Slate/SlateGameResources.h" // 동적 브러시 생성을 위해 추가


void SStartupLoadingScreen::Construct(const FArguments& InArgs)
{
    // 1. 멤버 변수에 저장된 경로로 로드
    UTexture2D* LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *LogoPath));
    // 2. 로드된 텍스처로 브러시 생성
    if (LoadedTexture)
    {
        LogoBrush = MakeShareable(new FSlateImageBrush(LoadedTexture,ImageSize));
    }

    // 4. 애니메이션 커브 설정
    FadeInCurve = FCurveSequence(); // 애니메이션 설정
    FadeInCurve.AddCurve(AnimationTiming.X, AnimationTiming.Y, ECurveEaseFunction::QuadIn); // 0 ~ t초 동안 진행되는 커브 생성
    FadeInCurve.Play(this->AsShared()); // 애니메이션 시작

    // "월드가 없는 상태"에서도 안전하게 도는 Slate UI
    ChildSlot
        [
            SNew(SOverlay) // 중첩 배치를 위해 Overlay 사용
                // 1. 배경 (검정색)
                + SOverlay::Slot()
                [
                    SNew(SBorder)
                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
                        .BorderBackgroundColor(FLinearColor::Black)
                ]

                // 2. 중앙 로고 (페이드 효과 적용)
                + SOverlay::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(SBox)
                        .WidthOverride(ImageSize.X)
                        .HeightOverride(ImageSize.Y)
                        [
                            SNew(SImage)
                                .Image(LogoBrush.Get())
                                // GetLogoColorAndOpacity 함수를 통해 실시간으로 투명도 변화
                                .ColorAndOpacity(this, &SStartupLoadingScreen::GetLogoColorAndOpacity)
                        ]
                ]

            // 3. 우측 하단 로딩 표시
            + SOverlay::Slot()
                .HAlign(HAlign_Right)
                .VAlign(VAlign_Bottom)
                .Padding(0, 0, 50.0f, 50.0f) // 구석에서 적절히 띄움
                [
                    SNew(SThrobber)
                        .NumPieces(3)
                ]

            
        ];
}

// 투명도 계산 함수 구현
FSlateColor SStartupLoadingScreen::GetLogoColorAndOpacity() const
{
    // 커브의 진행도(0.0 ~ 1.0)를 가져와서 알파값에 적용
    float Progress = FadeInCurve.GetLerp();
    return FLinearColor(1.0f, 1.0f, 1.0f, Progress);
}