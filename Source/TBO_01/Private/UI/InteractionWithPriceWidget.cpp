// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionWithPriceWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInteractionWithPriceWidget::SetupInteraction(float Price)
{
	const bool bShowPrice = Price > 0.0f;

	UE_LOG(LogTemp, Display, TEXT("Price : %f"), Price);
	
	if (PriceText)
	{
		PriceText->SetText(FText::AsNumber(Price));
		PriceText->SetVisibility(bShowPrice ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (PriceIcon)
	{
		PriceIcon->SetVisibility(bShowPrice ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}