#pragma once
#include "GenericTeamAgentInterface.h"
#include "TeamAffiliationConfig.generated.h"

UENUM(BlueprintType)
enum class EGameTeam : uint8
{
	Default     UMETA(DisplayName = "Default"),  
	Player      UMETA(DisplayName = "Player"),
	Pacific     UMETA(DisplayName = "Pacific"),
	Aggressive  UMETA(DisplayName = "Aggressive"),
	Other       UMETA(DisplayName = "Other"),
	Max         UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FTeamAffiliationConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamAffiliationConfig")
	EGameTeam Team = EGameTeam::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamAffiliationConfig")
	TMap<EGameTeam, TEnumAsByte<ETeamAttitude::Type>> RelationMap;

	//constructor
	FTeamAffiliationConfig()
	{
		RelationMap.Add(EGameTeam::Player, ETeamAttitude::Neutral);
		RelationMap.Add(EGameTeam::Pacific, ETeamAttitude::Neutral);
		RelationMap.Add(EGameTeam::Aggressive, ETeamAttitude::Neutral);
		RelationMap.Add(EGameTeam::Other, ETeamAttitude::Neutral);
	}
	
	/**Return the relationship with the passed Team */
	ETeamAttitude::Type GetTeamAttitude(EGameTeam OtherTeam) const
	{
		if(RelationMap.Contains(OtherTeam))
		{
			return RelationMap[OtherTeam];
		}
		return ETeamAttitude::Neutral;
	}
	FGenericTeamId GetTeam() const
	{
		return  FGenericTeamId(static_cast<uint8>(Team));
	}

};
