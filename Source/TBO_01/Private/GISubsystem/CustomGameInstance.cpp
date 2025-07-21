#include "GISubsystem/CustomGameInstance.h"
#include "Engine/DataTable.h"

void UCustomGameInstance::Init()
{
	Super::Init();
	MySubsystem = GetSubsystem<USceneHandlerGISubsystem>();
}

USceneHandlerGISubsystem* UCustomGameInstance::GetSceneHandlerSubsystem() const
{
	return MySubsystem;
}
