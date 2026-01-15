// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/SBaseLoadingWidget.h"

class SEntryLoadingWidget : public SBaseLoadingWidget
{
public:
	SLATE_BEGIN_ARGS(SEntryLoadingWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> BuildCenterContent() override;
};