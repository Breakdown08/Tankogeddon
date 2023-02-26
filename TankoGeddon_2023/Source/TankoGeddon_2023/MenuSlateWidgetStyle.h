// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "Styling/SlateTypes.h"

#include "MenuSlateWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct TANKOGEDDON_2023_API FMenuSlateStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FMenuSlateStyle();
	virtual ~FMenuSlateStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FMenuSlateStyle& GetDefault();

	/** Style of options text */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FTextBlockStyle TextStyle;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UMenuSlateWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FMenuSlateStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
