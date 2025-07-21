// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CreatureEnumHelper.generated.h"

UENUM(BlueprintType)
enum class ENoiseAction : uint8
{
	None       UMETA(DisplayName="None"),
	ScarySound    UMETA(DisplayName="ScarySound"),
	NormalSound   UMETA(DisplayName="NormalSound"),
	ChargedSound   UMETA(DisplayName="ChargedSound"),
	CoinSound   UMETA(DisplayName="CoinSound"),
	
};
UENUM(BlueprintType)
enum class ETouchAction : uint8
{
	None       UMETA(DisplayName="None"),
	BodyTouch  UMETA(DisplayName="BodyTouch"),
	PartTouch  UMETA(DisplayName="PartTouch"),
};
UENUM(BlueprintType)
enum class EDamageAction : uint8
{
	None       UMETA(DisplayName="None"),
	TriggerBoxDamage UMETA(DisplayName="TriggerBoxDamage"),
	HurtBoxDamage UMETA(DisplayName="HurtBoxDamage"),
	DestructionDamage UMETA(DisplayName="DestructionDamage"),
	EyesDamage UMETA(DisplayName="EyesDamage"),
};
/**
 * 
 */
UCLASS()
class TBO_01_API UCreatureEnumHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Enum to Fname*/
	template <typename TEnum>
	static FName EnumToFName(TEnum EnumValue);

	/** Fname to Enum */
	template <typename TEnum>
	static TEnum FNameToEnum(const FName& EnumName);

	/** Enum Array to Fname */
	template <typename TEnum>
	static FName EnumArrayToString(const TArray<TEnum>& EnumArray, const FString& Separator = TEXT(","));

	/** Fname to Enum Array */
	template <typename TEnum>
	static TArray<TEnum> StringToEnumArray(const FString& EnumString, const FString& Separator = TEXT(","));
};