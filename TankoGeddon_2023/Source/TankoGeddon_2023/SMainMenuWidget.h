// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSlateWidgetStyle.h"
#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SMainMenuWidget: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_STYLE_ARGUMENT(FMenuSlateStyle, Style)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetMenuStyle(const FMenuSlateStyle* InStyle);
	
	FReply OnPlayClicked() const;
	FReply OnQuitClicked() const;
	
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override {return true;};

protected:
	/** Style resource for text */
	const FTextBlockStyle* TextStyle = nullptr;
};
