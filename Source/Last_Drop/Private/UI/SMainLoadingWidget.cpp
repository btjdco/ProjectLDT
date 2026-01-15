// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SMainLoadingWidget.h"
#include "Internationalization/Text.h"

//#include "Widgets/SOverlay.h" // *바뀐듯?

void SMainLoadingWidget::Construct(const FArguments& InArgs)
{
	InitTipKeys();
	PickRandomTip();

	SBaseLoadingWidget::Construct(
		SBaseLoadingWidget::FArguments()
		.LogoPath(TEXT("/Game/NewProject/Rendering/UMG/T_LastDropLogo.T_LastDropLogo"))
		.LogoSize(FVector2D(300.f, 300.f))
		.FadeIn(FVector2D(0.f, 1.00f))
		.FadeOut(FVector2D(0.f, 1.00f))
		.ShowIndicator(true)
		.IndicatorPadding(FMargin(0, 0, 28, 22))
		.BackgroundColor(FLinearColor(0.05f, 0.05f, 0.08f, 1.0f)) // 예: 아주 어두운 남색
	);
}

void SMainLoadingWidget::InitTipKeys()
{
	// 테스트 2개 (나중에 10개로 늘리면 됨)
	TipKeys =
	{
		TEXT("A"),
		TEXT("B"),
	};
}

void SMainLoadingWidget::PickRandomTip()
{
	if (TipKeys.Num() == 0)
	{
		CurrentTip = FText::GetEmpty();
		return;
	}

	const int32 Index = FMath::RandRange(0, TipKeys.Num() - 1);
	UE_LOG(LogTemp, Warning, TEXT("TickCounter: %s"), *TipStringTableId.ToString());
	UE_LOG(LogTemp, Warning, TEXT("TickCounter: %s"), *TipKeys[Index].ToString());

	CurrentTip = FText::FromStringTable(TipStringTableId, TipKeys[Index].ToString());
}

TSharedRef<SWidget> SMainLoadingWidget::BuildCenterContent()
{
	// 로고 아래 ~ 화면 하단 사이 느낌: VerticalBox + padding
	return SNew(SVerticalBox)

		// 로고
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(SBox)
				.WidthOverride(300.f)
				.HeightOverride(300.f)
				[
					BuildLogoWidget()
				]
		]

	// 간격
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(0, 16, 0, 0))
		.HAlign(HAlign_Center)
		[
			SNew(SBorder)
				.Padding(FMargin(18, 12))
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.35f))
				[
					SNew(STextBlock)
						.Text(CurrentTip)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
						.Justification(ETextJustify::Center)
						.WrapTextAt(900.f)
				]
		];
}