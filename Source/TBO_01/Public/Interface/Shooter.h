// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Shooter.generated.h"

class UMuzzlesController;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UShooter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TBO_01_API IShooter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shooter Interface")
	UMuzzlesController* GetMuzzlesController();
};
