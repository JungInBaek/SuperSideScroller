// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperSideScrollerCharacter.h"
#include "EnermyBase.generated.h"

/**
 * 
 */
UCLASS()
class SUPERSIDESCROLLER_API AEnermyBase : public ASuperSideScrollerCharacter
{
	GENERATED_BODY()
	
public:
	void DestroyEnermy();
};
