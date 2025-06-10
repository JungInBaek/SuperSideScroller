// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SuperSideScroller_Player.h"
#include "Components/CapsuleComponent.h"


void AEnermyBase::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnermyBase::OnHit);
}

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

void AEnermyBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Enermy Hit"));

	ASuperSideScroller_Player* Player = Cast<ASuperSideScroller_Player>(OtherActor);
	if (Player)
	{
		Player->DestroyPlayer();
	}
}
