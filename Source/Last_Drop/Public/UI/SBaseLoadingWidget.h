// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/StrongObjectPtr.h"
#include "Widgets/SCompoundWidget.h"

//#include "Misc/Optional.h"
//#include "Widgets/Layout/SBox.h"

//#include "Slate/SlateGameResources.h"
//#include "Animation/CurveSequence.h"

class SBaseLoadingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseLoadingWidget)
		: _LogoPath(TEXT(""))
		, _LogoSize(FVector2D(300.f, 300.f))
		, _FadeIn(FVector2D(0.f, 0.35f))
		, _FadeOut(FVector2D(0.f, 0.35f))
		, _ShowIndicator(true)
		, _IndicatorPadding(FMargin(0, 0, 28, 22))
		, _BackgroundColor(FLinearColor::Black)
		{
		}
		SLATE_ARGUMENT(FString, LogoPath)
		SLATE_ARGUMENT(FVector2D, LogoSize)

		SLATE_ARGUMENT(FVector2D, FadeIn)
		SLATE_ARGUMENT(FVector2D, FadeOut)

		SLATE_ARGUMENT(bool, ShowIndicator)
		SLATE_ARGUMENT(FMargin, IndicatorPadding)

		SLATE_ARGUMENT(FLinearColor, BackgroundColor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/** 로딩 끝나기 직전에 호출하면 1->0 페이드 아웃 */
	void StartFadeOut();

protected:
	// ==== A~D 공용 파이프라인 ====
	bool PrepareLoadingResources(const FString& InPath, const FVector2D& InSize);
	void SetupAndPlayFadeInOut(const FVector2D& InFadeIn, const FVector2D& InFadeOut);

	// 로고 알파(페이드) 바인딩용
	FSlateColor GetLogoColorAndOpacity() const;
	float GetFadeAlpha() const;

	// ==== 공용 UI 빌딩 블록 ====
	TSharedRef<SWidget> BuildRoot();        // 전체 화면(배경 + 슬롯)
	TSharedRef<SWidget> BuildLogoWidget();  // 로고 이미지 위젯

	/** 자식이 “중앙 컨텐츠”만 구성하면 됨 (로고만 / 로고+팁 등) */
	virtual TSharedRef<SWidget> BuildCenterContent() = 0;
	virtual TSharedRef<SWidget> BuildIndicatorWidget();

protected:
	// 리소스
	TSharedPtr<FSlateBrush> LogoBrush;
	TStrongObjectPtr<UTexture2D> LoadedTexture;
	bool bHasValidLogo = false;

	// 페이드
	FCurveSequence FadeSequence;

	// 배경
	FLinearColor BackgroundColor = FLinearColor::Black;

	// 인디케이터 옵션
	bool bShowIndicator = true;
	FMargin IndicatorPadding = FMargin(0, 0, 28, 22);
};
