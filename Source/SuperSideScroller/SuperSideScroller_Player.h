// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperSideScrollerCharacter.h"
#include "SuperSideScroller_Player.generated.h"


UCLASS()
class SUPERSIDESCROLLER_API ASuperSideScroller_Player : public ASuperSideScrollerCharacter
{
	GENERATED_BODY()
	
public:
	ASuperSideScroller_Player();

	void SpawnProjectile();

	UFUNCTION(BlueprintPure)
	int32 GetCurrentNumberOfCollectables() { return NumberOfCollectables; }

	void IncrementNumberOfCollectables(int32 Value);

	void IncreaseMovementPowerup();

	void DecreaseMovementPowerdown();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Sprint();

	void StopSprinting();

	void ThrowProjectile();

	void EndPowerup();

	void EndPowerdown();

private:
	bool bIsSprinting;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* ThrowMontage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APlayerProjectile> PlayerProjectile;

	int32 NumberOfCollectables;

	FTimerHandle PowerupHandle;

	bool bHasPowerupActive;

	bool bHasPowerdownActive;

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* IC_Character;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Sprint;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Throw;
};
