// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!OwnerUnit.IsValid())
	{
		return;
	}

	HealthBar->SetPercent(OwnerUnit->GetHealth() / OwnerUnit->GetMaxHealth());
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerUnit->GetHealth(), &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(OwnerUnit->GetMaxHealth(), &Opts));
}
