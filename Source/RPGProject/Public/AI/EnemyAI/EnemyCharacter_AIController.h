// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "EnemyCharacter_AIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerCharacterSeen, AActor*, SeenPlayerCharacter);

class AEnemyCharacter;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AEnemyCharacter_AIController : public AAIController
{
	GENERATED_BODY()

	public:
		AEnemyCharacter_AIController(const FObjectInitializer& object_intializer = FObjectInitializer::Get());
		virtual void BeginPlay() override;
		virtual void OnPossess(APawn* InPawn) override;
		UBlackboardComponent* GetBlackboard() const;

		UPROPERTY(BlueprintAssignable)
		FPlayerCharacterSeen PlayerCharacterSeen;
	
	private:
		UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="AI",meta =(AllowPrivateAccess="true"))
		UBehaviorTreeComponent* Behavior_Tree_Component;

		UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="AI",meta =(AllowPrivateAccess="true"))
		UBehaviorTree* Behavior_Tree;

		UBlackboardComponent* BlackBoardComp;

		float PerceptionTime;

		UAISenseConfig_Sight* EnemeySightComp;

		UAISenseConfig_Hearing* EnemyHearingComp;

		AEnemyCharacter* AI_Controller_Owner=nullptr;

		UFUNCTION()
		void OnPerceptionsUpdated(TArray<AActor*> const& updated_actors);

		UFUNCTION()
		void OnAIDie(AActor* VictimActor, AActor* KillerActor, AController* KillerController);

		void setup_perception_system();
	
};
