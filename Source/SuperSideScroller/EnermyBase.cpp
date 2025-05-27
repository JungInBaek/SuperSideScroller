// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


void AEnermyBase::DestroyEnermy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (DeathEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, DeathEffect, GetActorTransform());
		}
		if (DeathSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(World, DeathSound, GetActorLocation());
		}
	}
	Destroy();
}
