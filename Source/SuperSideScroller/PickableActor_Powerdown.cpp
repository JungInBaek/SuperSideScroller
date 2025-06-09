// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor_Powerdown.h"
#include "SuperSideScroller_Player.h"


void APickableActor_Powerdown::BeginPlay()
{
	Super::BeginPlay();
}

void APickableActor_Powerdown::PlayerPickedUp(ASuperSideScroller_Player* Player)
{
	Player->DecreaseMovementPowerdown();
	Super::PlayerPickedUp(Player);
}
