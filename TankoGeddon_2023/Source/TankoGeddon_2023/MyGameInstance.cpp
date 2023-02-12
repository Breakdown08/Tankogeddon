// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidget(TEXT("/Game/Core/Widgets/WBP_GameOver"));
	if (!ensure (GameOverWidget.Class != nullptr)) return;

	GameOverWidgetClass = GameOverWidget.Class;
	
}

void UMyGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("found %s"), *GameOverWidgetClass->GetName())
}

void UMyGameInstance::ShowWidget()
{
	UUserWidget* GameOver = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
	GameOver->AddToViewport();
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(GameOver->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

}
