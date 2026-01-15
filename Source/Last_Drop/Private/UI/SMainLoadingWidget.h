// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/SBaseLoadingWidget.h"

class SMainLoadingWidget : public SBaseLoadingWidget
{
public:
	SLATE_BEGIN_ARGS(SMainLoadingWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> BuildCenterContent() override;

private:
	void InitTipKeys();
	void PickRandomTip();
	FText GetTipText() const { return CurrentTip; }

private:
	FText CurrentTip;

	TArray<FName> TipKeys;
	FName TipStringTableId = TEXT("ST_LoadingTips"); // 너가 만든 String Table ID

};