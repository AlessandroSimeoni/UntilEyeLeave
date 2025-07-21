// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseCreatureAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TBO_01_API UBaseCreatureAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,Health);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,Attack);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,CharacterLevel);
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData KnockBackIntensity;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,KnockBackIntensity);
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,Speed);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData RotationSpeed;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,RotationSpeed);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData EyesHitPoint;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,EyesHitPoint);
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData EyeLevel;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,EyeLevel);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData EyeTransitionTime;
	ATTRIBUTE_ACCESSORS(UBaseCreatureAttributeSet,EyeTransitionTime);

	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	
	
};
