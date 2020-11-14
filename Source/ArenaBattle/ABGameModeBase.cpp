// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameModeBase.h"
#include "ABPawn.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"

AABGameModeBase::AABGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Book/Animations/MyCharacter"));

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	//DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameModeBase::PostLogin(APlayerController *NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}

