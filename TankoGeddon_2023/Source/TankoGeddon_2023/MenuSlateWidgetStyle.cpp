// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSlateWidgetStyle.h"

FMenuSlateStyle::FMenuSlateStyle()
{
}

FMenuSlateStyle::~FMenuSlateStyle()
{
}

const FName FMenuSlateStyle::TypeName(TEXT("FMenuSlateStyle"));

const FMenuSlateStyle& FMenuSlateStyle::GetDefault()
{
	static FMenuSlateStyle Default;
	return Default;
}

void FMenuSlateStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

