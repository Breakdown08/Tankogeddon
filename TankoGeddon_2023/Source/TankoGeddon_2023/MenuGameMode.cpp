// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"

#include "MenuHUD.h"
#include "MenuPlayerController.h"

AMenuGameMode::AMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Core/BP_TankPawn"));
	static ConstructorHelpers::FClassFinder<AMenuHUD> HUDObjectFinder(TEXT("Blueprint'/Game/Core/Widgets/WBP_MenuHUD'"));
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AMenuPlayerController::StaticClass();
	//HUDClass = AMenuHUD::StaticClass();
	HUDClass = HUDObjectFinder.Class;
}
