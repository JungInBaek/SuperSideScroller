// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperSideScroller_Player.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "PlayerProjectile.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "SuperSideScroller_Controller.h"


ASuperSideScroller_Player::ASuperSideScroller_Player()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void ASuperSideScroller_Player::IncrementNumberOfCollectables(int32 Value)
{
	if (Value <= 0)
	{
		return;
	}
	NumberOfCollectables += Value;
	//UE_LOG(LogTemp, Warning, TEXT("Number of Coins: %d"), NumberOfCollectables);
	if (NumberOfCollectables >= 10 && NumberOfCollectables % 10 == 0)
	{
		IncreaseMovementPowerup();
	}
}

void ASuperSideScroller_Player::IncreaseMovementPowerup()
{
	bHasPowerupActive = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 1500.0f;

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(PowerupHandle, this, &ASuperSideScroller_Player::EndPowerup, 8.0f, false);
	}
}

void ASuperSideScroller_Player::DecreaseMovementPowerdown()
{
	bHasPowerdownActive = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(PowerupHandle, this, &ASuperSideScroller_Player::EndPowerdown, 8.0f, false);
	}
}

void ASuperSideScroller_Player::DestroyPlayer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(PowerupHandle);
		if (DeathEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, DeathEffect, GetActorTransform());
		}
		if (DeathSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(World, DeathSound, GetActorLocation());
		}
	}
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, ThrowMontage);
	}

	if (ASuperSideScroller_Controller* PlayerController = Cast<ASuperSideScroller_Controller>(GetController()))
	{
		PlayerController->ShowRestartWidget();
	}
	//Destroy();
}

void ASuperSideScroller_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedPlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedPlayerInput)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (EnhancedSubsystem)
		{
			EnhancedSubsystem->AddMappingContext(IC_Character, 1);
			
			EnhancedPlayerInput->BindAction(IA_Sprint, ETriggerEvent::Started, this, &ASuperSideScroller_Player::Sprint);
			EnhancedPlayerInput->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ASuperSideScroller_Player::StopSprinting);
			EnhancedPlayerInput->BindAction(IA_Throw, ETriggerEvent::Triggered, this, &ASuperSideScroller_Player::ThrowProjectile);
		}
	}
}

void ASuperSideScroller_Player::Sprint()
{
	if (bIsSprinting == false)
	{
		bIsSprinting = true;
		if (bHasPowerupActive)
		{
			GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		}
		else if (bHasPowerdownActive)
		{
			return;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
	}
}

void ASuperSideScroller_Player::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		if (bHasPowerupActive)
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		}
	}
}

void ASuperSideScroller_Player::ThrowProjectile()
{
	if (ThrowMontage)
	{
		bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(ThrowMontage);
		if (bIsMontagePlaying == false)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(ThrowMontage, 1.0f);
		}
	}
}

void ASuperSideScroller_Player::EndPowerup()
{
	bHasPowerupActive = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(PowerupHandle);
	}
}

void ASuperSideScroller_Player::EndPowerdown()
{
	bHasPowerdownActive = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(PowerupHandle);
	}
}

void ASuperSideScroller_Player::SpawnProjectile()
{
	if (PlayerProjectile)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			const FVector SpawnLocation = this->GetMesh()->GetSocketLocation(FName("ProjectileSocket"));
			const FRotator Rotation = GetActorForwardVector().Rotation();

			APlayerProjectile* Projectile = World->SpawnActor<APlayerProjectile>(PlayerProjectile, SpawnLocation, Rotation, SpawnParams);
		}
	}
}
