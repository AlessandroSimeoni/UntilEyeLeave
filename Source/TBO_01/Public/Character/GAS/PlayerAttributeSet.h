// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FonCoinChanged, float, NewValue,float,OldValue);
UCLASS()
class TBO_01_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Attribute")
	FonCoinChanged OnCoinChanged;
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,Attack);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData WeaponLevel;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,WeaponLevel);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData Coin;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,Coin);
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	
};
