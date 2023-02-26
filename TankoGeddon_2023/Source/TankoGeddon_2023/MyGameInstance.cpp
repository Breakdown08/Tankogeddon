// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameStructs.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidget(TEXT("/Game/Core/Widgets/WBP_GameOver"));
	if (!ensure (GameOverWidget.Class != nullptr)) return;
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidget(TEXT("/Game/Core/Widgets/WBP_MainMenu"));
	if (!ensure (GameOverWidget.Class != nullptr)) return;
	
	GameOverWidgetClass = GameOverWidget.Class;
	MainMenuWidgetClass = MainMenuWidget.Class;
}

void UMyGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("found %s"), *GameOverWidgetClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("found %s"), *MainMenuWidgetClass->GetName());
	
}


void UMyGameInstance::ShowWidget(EWidgetType WidgetType)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(this, GetWidgetByType(WidgetType));
	Widget->AddToViewport();
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Widget->TakeWidget());
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//PlayerController->SetInputMode(InputModeData);
	//PlayerController->bShowMouseCursor = true;

}

TSubclassOf<UUserWidget> UMyGameInstance::GetWidgetByType(EWidgetType WidgetType)
{
	switch (WidgetType)
	{ 
		case EWidgetType::GameOver:
			return GameOverWidgetClass; 
		case EWidgetType::MainMenu:
			return MainMenuWidgetClass; 
		default: 
			return MainMenuWidgetClass; 
	}
}

void UMyGameInstance::OnStart()
{
	Super::OnStart();
	//ShowWidget(EWidgetType::MainMenu);
}
