// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/EnemyCharacter_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/PlayerCharacter/PlayerCharacter.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "blackboard_keys.h"
#include "AI/EnemyCharacter/EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyCharacter_AIController::AEnemyCharacter_AIController(const FObjectInitializer& object_intializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Blueprint/Enemy/ECharacter_BT.ECharacter_BT'"));
	if(obj.Succeeded())
	{
		Behavior_Tree = obj.Object;
	}
	Behavior_Tree_Component = object_intializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this,TEXT("BehaviorComponent"));
	BlackBoardComp = object_intializer.CreateDefaultSubobject<UBlackboardComponent>(this,TEXT("BlackboardComp"));
	setup_perception_system();
}

void AEnemyCharacter_AIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(Behavior_Tree);
	Behavior_Tree_Component->StartTree(*Behavior_Tree);
	AI_Controller_Owner = Cast<AEnemyCharacter>(GetPawn());
	if(AI_Controller_Owner!= nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Owner is ready"));
		AI_Controller_Owner->OnEnemyDie.AddDynamic(this,&AEnemyCharacter_AIController::OnAIDie);
	}
}

void AEnemyCharacter_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(BlackBoardComp)
	{
		BlackBoardComp->InitializeBlackboard(*Behavior_Tree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemyCharacter_AIController::GetBlackboard() const
{
	return BlackBoardComp;
}

void AEnemyCharacter_AIController::OnPerceptionsUpdated(TArray<AActor*> const& updated_actors)
{
	for(int i=0;i<updated_actors.Num();i++)
	{
		FActorPerceptionBlueprintInfo BP_Info;
		GetPerceptionComponent()->GetActorsPerception(updated_actors[i],BP_Info);
		for(int y=0; y < BP_Info.LastSensedStimuli.Num();y++)
		{
			FAIStimulus Stim = BP_Info.LastSensedStimuli[y];
			if(Stim.Tag == "Noise")
			{
				GetBlackboardComponent()->SetValueAsVector(bb_keys::target_location,Stim.StimulusLocation);
				GetBlackboardComponent()->SetValueAsBool(bb_keys::is_investigating,Stim.WasSuccessfullySensed());
			}
			else if(Stim.Type.Name == "Default__AISense_Sight")
			{
				APlayerCharacter* SeenCharacter = Cast<APlayerCharacter>(updated_actors[i]);
				if(SeenCharacter)
				{
					GetBlackboardComponent()->SetValueAsBool(bb_keys::can_see_player,Stim.WasSuccessfullySensed());
					PlayerCharacterSeen.Broadcast(updated_actors[i]);
				}
			}
		}
	}
}

void AEnemyCharacter_AIController::OnAIDie(AActor* VictimActor, AActor* KillerActor, AController* KillerController)
{
	UE_LOG(LogTemp,Warning,TEXT("AI Unpossed"));
	BlackBoardComp->SetValueAsBool("IsAlive",false);
	BrainComponent->StopLogic("Dead");
	Behavior_Tree_Component->StopTree(EBTStopMode::Safe);
	UnPossess();
	Destroy();
}

void AEnemyCharacter_AIController::setup_perception_system()
{
	EnemeySightComp = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(EnemeySightComp)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Comp")));
		EnemeySightComp->SightRadius = 6000.0f;
		EnemeySightComp->LoseSightRadius = EnemeySightComp->SightRadius + 100.0f;
		EnemeySightComp->PeripheralVisionAngleDegrees=45.0f;
		EnemeySightComp->SetMaxAge(5.0f);
		EnemeySightComp->AutoSuccessRangeFromLastSeenLocation = 900.0f;
		EnemeySightComp->DetectionByAffiliation.bDetectEnemies=true;
		EnemeySightComp->DetectionByAffiliation.bDetectFriendlies=true;
		EnemeySightComp->DetectionByAffiliation.bDetectNeutrals=true;
		GetPerceptionComponent()->SetDominantSense(*EnemeySightComp->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*EnemeySightComp);
	}

	EnemyHearingComp = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if(EnemyHearingComp)
	{
		EnemyHearingComp->HearingRange = 20000.0f;
		EnemyHearingComp->DetectionByAffiliation.bDetectEnemies=true;
		EnemyHearingComp->DetectionByAffiliation.bDetectFriendlies=true;
		EnemyHearingComp->DetectionByAffiliation.bDetectNeutrals=true;
		EnemyHearingComp->SetMaxAge(5.0f);

		
		GetPerceptionComponent()->ConfigureSense(*EnemyHearingComp);
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this,&AEnemyCharacter_AIController::OnPerceptionsUpdated);
	}
}


