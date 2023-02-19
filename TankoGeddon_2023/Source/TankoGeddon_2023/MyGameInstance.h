// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameStructs.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_2023_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	void ShowWidget(EWidgetType WidgetType);
	TSubclassOf<class UUserWidget> GetWidgetByType(EWidgetType WidgetType);
	void OnStart() override;

private:
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
};