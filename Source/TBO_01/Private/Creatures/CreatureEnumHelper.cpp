// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/CreatureEnumHelper.h"

template <typename TEnum>
FName UCreatureEnumHelper::EnumToFName(TEnum EnumValue)
{
    UEnum* EnumPtr = StaticEnum<TEnum>();
    if (!EnumPtr) return NAME_None;

    return EnumPtr->GetNameByValue(static_cast<int64>(EnumValue));
}

template <typename TEnum>
TEnum UCreatureEnumHelper::FNameToEnum(const FName& EnumName)
{
    UEnum* EnumPtr = StaticEnum<TEnum>();
    if (!EnumPtr) return static_cast<TEnum>(0);

    int64 EnumValue = EnumPtr->GetValueByName(EnumName);
    return static_cast<TEnum>(EnumValue);
}

template <typename TEnum>
FName UCreatureEnumHelper::EnumArrayToString(const TArray<TEnum>& EnumArray, const FString& Separator)
{
    UEnum* EnumPtr = StaticEnum<TEnum>();
    if (!EnumPtr) return TEXT("");

    FString Result;
    for (TEnum EnumValue : EnumArray)
    {
        FString EnumName = EnumPtr->GetNameByValue(static_cast<int64>(EnumValue)).ToString();
        Result += EnumName + Separator;
    }

    if (!Result.IsEmpty()) 
    {
        Result.RemoveFromEnd(Separator); 
    }

    return FName(*Result);
}

template <typename TEnum>
TArray<TEnum> UCreatureEnumHelper::StringToEnumArray(const FString& EnumString, const FString& Separator)
{
    UEnum* EnumPtr = StaticEnum<TEnum>();
    if (!EnumPtr) return {};

    TArray<TEnum> EnumArray;
    TArray<FString> EnumNames;
    EnumString.ParseIntoArray(EnumNames, *Separator, true);

    for (const FString& EnumName : EnumNames)
    {
        int64 EnumValue = EnumPtr->GetValueByName(FName(*EnumName));
        if (EnumPtr->IsValidEnumValue(EnumValue))
        {
            EnumArray.Add(static_cast<TEnum>(EnumValue));
        }
    }

    return EnumArray;
}


template FName UCreatureEnumHelper::EnumToFName<ENoiseAction>(ENoiseAction EnumValue);
template ENoiseAction UCreatureEnumHelper::FNameToEnum<ENoiseAction>(const FName& EnumName);
template FName UCreatureEnumHelper::EnumArrayToString<ENoiseAction>(const TArray<ENoiseAction>& EnumArray, const FString& Separator);
template TArray<ENoiseAction> UCreatureEnumHelper::StringToEnumArray<ENoiseAction>(const FString& EnumString, const FString& Separator);
template FName UCreatureEnumHelper::EnumToFName<EDamageAction>(EDamageAction EnumValue);
template EDamageAction UCreatureEnumHelper::FNameToEnum<EDamageAction>(const FName& EnumName);
template FName UCreatureEnumHelper::EnumArrayToString<EDamageAction>(const TArray<EDamageAction>& EnumArray, const FString& Separator);
template TArray<EDamageAction> UCreatureEnumHelper::StringToEnumArray<EDamageAction>(const FString& EnumString, const FString& Separator);
