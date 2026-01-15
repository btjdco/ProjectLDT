// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SBaseLoadingWidget.h"

//#include "Styling/SlateBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/CoreStyle.h"
#include "Engine/Texture2D.h"


void SBaseLoadingWidget::Construct(const FArguments& InArgs)
{
	bShowIndicator = InArgs._ShowIndicator;
	IndicatorPadding = InArgs._IndicatorPadding;
	BackgroundColor = InArgs._BackgroundColor;

	bHasValidLogo = PrepareLoadingResources(InArgs._LogoPath, InArgs._LogoSize);
	SetupAndPlayFadeInOut(InArgs._FadeIn, InArgs._FadeOut);

	ChildSlot
		[
			BuildRoot()
		];
}

bool SBaseLoadingWidget::PrepareLoadingResources(const FString& InPath, const FVector2D& InSize)
{
	if (InPath.IsEmpty() || InSize.IsZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingWidget: invalid path/size."));
		LogoBrush.Reset();
		LoadedTexture.Reset();
		return false;
	}

	UTexture2D* Tex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *InPath));
	if (!Tex)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadingWidget: Failed to load texture: %s"), *InPath);
		LogoBrush.Reset();
		LoadedTexture.Reset();
		return false;
	}

	LoadedTexture = TStrongObjectPtr<UTexture2D>(Tex);

	// 안전한 방식: FSlateBrush에 ResourceObject 연결
	TSharedPtr<FSlateBrush> Brush = MakeShared<FSlateBrush>();
	Brush->SetResourceObject(Tex);
	Brush->ImageSize = InSize;

	LogoBrush = Brush;
	return LogoBrush.IsValid();
}

void SBaseLoadingWidget::SetupAndPlayFadeInOut(const FVector2D& InFadeIn, const FVector2D& InFadeOut)
{
	FadeSequence = FCurveSequence();

	// Fade In (0 -> 1)
	FadeSequence.AddCurve(InFadeIn.X, InFadeIn.Y, ECurveEaseFunction::QuadInOut);

	// Fade Out은 StartFadeOut에서 Reverse로 처리
	FadeSequence.Play(this->AsShared());
}

void SBaseLoadingWidget::StartFadeOut()
{
	// 1->0: 끝으로 점프 후 Reverse
	FadeSequence.JumpToEnd();
	FadeSequence.PlayReverse(this->AsShared());
}

float SBaseLoadingWidget::GetFadeAlpha() const
{
	if (!LogoBrush.IsValid())
		return 0.f;

	return FadeSequence.GetLerp(); // Play: 0->1, Reverse: 1->0
}

FSlateColor SBaseLoadingWidget::GetLogoColorAndOpacity() const
{
	return FLinearColor(1.f, 1.f, 1.f, GetFadeAlpha());
}

TSharedRef<SWidget> SBaseLoadingWidget::BuildLogoWidget()
{
	if (!LogoBrush.IsValid())
	{
		return SNullWidget::NullWidget;
	}

	return SNew(SImage)
		.Image(LogoBrush.Get())
		.ColorAndOpacity(this, &SBaseLoadingWidget::GetLogoColorAndOpacity);
}

TSharedRef<SWidget> SBaseLoadingWidget::BuildRoot()
{
	return SNew(SBorder)
		.Padding(0)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		.BorderBackgroundColor(BackgroundColor)
		[
			SNew(SOverlay)

				// 중앙 컨텐츠(자식이 정의)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					BuildCenterContent()
				]

				// 우측 하단 Counter (공용)
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(IndicatorPadding)
				[
					bShowIndicator ? BuildIndicatorWidget() : SNullWidget::NullWidget
				]
		];
}

TSharedRef<SWidget> SBaseLoadingWidget::BuildIndicatorWidget()
{
	// 기본 인디케이터: 3개 점(원) 형태의 Throbber
	return SNew(SThrobber)
		.NumPieces(8)
		.Animate(SThrobber::EAnimation::All);
	// .PieceImage(...) 로 커스텀 이미지도 가능
}