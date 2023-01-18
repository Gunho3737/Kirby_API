#include "Stage_one_one.h"
#include "Normal_Kirby.h"
#include "One_One_Back.h"
#include <GH_GameEngineImage.h>
#include <GH_ResourcesManager.h>
#include <GH_GameEngineDirectory.h>
#include "WeakMonster.h"
#include "One_One_Far.h"
#include "BeamMonster.h"
#include "CutterMonster.h"
#include "TreeBoss.h"
#include <GH_GameEngineSoundPlayer.h>
#include <GH_GameEngineInput.h>
#include "TitleScreen.h"
#include <GH_GameEngineCollision.h>
#include "InGame_UI.h"

Normal_Kirby* Stage_one_one::NormalKirbyPtr = nullptr;
TreeBoss* Stage_one_one::TreeBossPtr = nullptr;
Stage_one_one* Stage_one_one::StagePtr = nullptr;

Stage_one_one::Stage_one_one()
	: CurStage(StageState::ONEONE)
{

}

Stage_one_one::~Stage_one_one()
{
}

Stage_one_one::Stage_one_one(Stage_one_one&& _Other) noexcept
{
}

void Stage_one_one::GH_Loading()
{

	FarPtr = GH_CreateActor<One_One_Far>();
	BackPtr = GH_CreateActor<One_One_Back>();
	NormalKirbyPtr = GH_CreateActor<Normal_Kirby>("Normal_Kirby");
	WeakMonsterPtr = GH_CreateActor<WeakMonster>();
	BeamMonsterPtr = GH_CreateActor<BeamMonster>();
	CutterMonsterPtr = GH_CreateActor<CutterMonster>();
	UIPtr = GH_CreateActor<InGame_UI>();
	StagePtr = this;

	{
		NormalKirbyPtr->GH_SetPos({ 150,100 });
	

		//좌우이동을 하는 빔/일반몬스터는 x값을 줘야한다
		WeakMonsterPtr->GH_SetPos({ 880,495 });
		WeakMonsterPtr->Movement = 880;
		BeamMonsterPtr->GH_SetPos({ 1200,495 });
		BeamMonsterPtr->Movement = 1200;
		
		BackPtr->MapCollision->GH_SetImage("Bit_1-1.bmp");
		BackPtr->StageMapState = StageState::ONEONE;
		//커터 몬스터의 경우 좌우이동없이 점프만 하므로 y값을 줘야한다
		CutterMonsterPtr->GH_SetPos({ 2750,410});
		CutterMonsterPtr->Movement = 410;

		UIPtr->GH_SetPos({0,0});

		BackPtr->DoorCollision->GH_SetPivot({ 1400, 450 });
		
	}

	BackgroundPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();

	if (nullptr == BackgroundPlayer)
	{
		GH_GameEngineDebug::Assert();
	}


}

void Stage_one_one::StageRestart()
{
	switch (CurStage)
	{
	case StageState::ONEONE:
	{
		{
			BackgroundPlayer->GH_Stop();
			BackgroundPlayer->GH_PlayAlone("Green_Greens.mp3", -1);

			BackPtr->MapCollision->GH_SetImage("Bit_1-1.bmp");
			BackPtr->StageMapState = StageState::ONEONE;
			FarPtr->MapState = StageState::ONEONE;

			NormalKirbyPtr->GH_SetPos({ 150,100 });
			NormalKirbyPtr->GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);
			NormalKirbyPtr->HP = 6;


			//좌우이동을 하는 빔/일반몬스터는 x값을 줘야한다
			if (false == WeakMonsterPtr->GH_IsOn())
			{
				WeakMonsterPtr->GH_On();
				WeakMonsterPtr->GH_ChangeState(GH_MonsterState::Walk);
			}
			WeakMonsterPtr->GH_SetPos({ 880,495 });
			WeakMonsterPtr->Movement = 880;
	
			if (false == BeamMonsterPtr->GH_IsOn())
			{
				BeamMonsterPtr->GH_On();
				BeamMonsterPtr->GH_ChangeState(GH_MonsterState::Walk);
			}
			BeamMonsterPtr->GH_SetPos({ 1200,495 });
			BeamMonsterPtr->Movement = 1200;
	

			if (false == CutterMonsterPtr->GH_IsOn())
			{
				CutterMonsterPtr->GH_On();
				CutterMonsterPtr->GH_ChangeState(GH_MonsterState::AttackReady);
			}
			CutterMonsterPtr->GH_SetPos({ 2750,410 });
			CutterMonsterPtr->Movement = 410;
			CutterMonsterPtr->GH_ChangeState(GH_MonsterState::AttackReady);

			UIPtr->GH_SetPos({ 0,0 });

			BackPtr->DoorCollision->GH_SetPivot({ 1400, 450 });
		
		}

	}
		break;
	case StageState::ONETWO:
	{
		BackgroundPlayer->GH_Stop();
		BackgroundPlayer->GH_PlayAlone("Green_Greens.mp3", -1);

		BackPtr->MapCollision->GH_SetImage("Bit_1-2.bmp");
		BackPtr->StageMapState = StageState::ONETWO;
		BackPtr->DoorCollision->GH_SetPivot({ 2800, 350 });
	

		NormalKirbyPtr->GH_SetPos({ 150,100 });
		NormalKirbyPtr->HP = 6;
		NormalKirbyPtr->GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);

		if (false == WeakMonsterPtr->GH_IsOn())
		{
			WeakMonsterPtr->GH_On();
			WeakMonsterPtr->GH_ChangeState(GH_MonsterState::Walk);
		}
		WeakMonsterPtr->GH_SetPos({ 880,580 });

		if (false == BeamMonsterPtr->GH_IsOn())
		{
			BeamMonsterPtr->GH_On();
			BeamMonsterPtr->GH_ChangeState(GH_MonsterState::Walk);
		}
		BeamMonsterPtr->GH_SetPos({ 1200,580 });

		if (false == CutterMonsterPtr->GH_IsOn())
		{
			CutterMonsterPtr->GH_On();
			CutterMonsterPtr->GH_ChangeState(GH_MonsterState::AttackReady);
		}
		CutterMonsterPtr->GH_SetPos({ 2500,600 });
	}
		break;
	case StageState::BOSS:
	{
		BackgroundPlayer->GH_Stop();
		BackgroundPlayer->GH_PlayAlone("Boss_Fight.mp3", -1);


		FarPtr->MapState = StageState::BOSS;

		BackPtr->StageMapState = StageState::BOSS;
		BackPtr->MapCollision->GH_SetImage("Bit_Boss.bmp");
		BackPtr->DoorCollision->GH_SetPivot({ -100, -100 });

		TreeBossPtr->GH_SetPos({ 650,450 });
		TreeBossPtr->HP = 9;
		TreeBossPtr->GH_ChangeState(GH_MonsterState::AttackReady);

		NormalKirbyPtr->GH_SetPos({ 150,100 });
		NormalKirbyPtr->HP = 6;
		NormalKirbyPtr->GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);

		if (true == WeakMonsterPtr->GH_IsOn())
		{
			WeakMonsterPtr->GH_Off();
			WeakMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
		}

		if (true == BeamMonsterPtr->GH_IsOn())
		{
			BeamMonsterPtr->GH_Dead();
			BeamMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
		}


		if (false == CutterMonsterPtr->GH_IsOn())
		{
			CutterMonsterPtr->GH_On();
			CutterMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
		}

	}
		break;
	default:
		break;
	}
}

void Stage_one_one::ChangeStage()
{
	switch (CurStage)
	{
	case StageState::ONEONE:
		CurStage = StageState::ONETWO;
		{
			FarPtr->MapState = StageState::ONETWO;

			BackPtr->MapCollision->GH_SetImage("Bit_1-2.bmp");
			BackPtr->StageMapState = StageState::ONETWO;
			BackPtr->DoorCollision->GH_SetPivot({ 2800, 350 });
			NormalKirbyPtr->GH_SetPos({ 150,100 });
			
			if (false == WeakMonsterPtr->GH_IsOn())
			{
				WeakMonsterPtr->GH_On();
				WeakMonsterPtr->GH_ChangeState(GH_MonsterState::Walk);
			}
			WeakMonsterPtr->GH_SetPos({ 880,580 });

			if (false == BeamMonsterPtr->GH_IsOn())
			{
				BeamMonsterPtr->GH_On();
				BeamMonsterPtr->GH_ChangeState(GH_MonsterState::Walk);
			}
			BeamMonsterPtr->GH_SetPos({ 1200,580 });

			if (false == CutterMonsterPtr->GH_IsOn())
			{
				CutterMonsterPtr->GH_On();
				CutterMonsterPtr->GH_ChangeState(GH_MonsterState::AttackReady);
			}
			CutterMonsterPtr->GH_SetPos({2500,420});
		}
		break;
	case StageState::ONETWO:
		CurStage = StageState::BOSS;
		{
			BackgroundPlayer->GH_Stop();
			BackgroundPlayer->GH_PlayAlone("Boss_Fight.mp3", -1);

			FarPtr->MapState = StageState::BOSS;

			BackPtr->StageMapState = StageState::BOSS;
			BackPtr->MapCollision->GH_SetImage("Bit_Boss.bmp");
			BackPtr->DoorCollision->GH_SetPivot({ -100, -100 });

			TreeBossPtr = GH_CreateActor<TreeBoss>();
			TreeBossPtr->GH_SetPos({650,450});

			NormalKirbyPtr->GH_SetPos({ 150,100 });

			if (true == WeakMonsterPtr->GH_IsOn())
			{
				WeakMonsterPtr->GH_Off();
				WeakMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
			}

			if (true == BeamMonsterPtr->GH_IsOn())
			{
				BeamMonsterPtr->GH_Off();
				BeamMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
			}


			if (false == CutterMonsterPtr->GH_IsOn())
			{
				CutterMonsterPtr->GH_Off();
				CutterMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
			}

		}
		break;
	case StageState::BOSS:
		CurStage = StageState::ONEONE;
		BackgroundPlayer->GH_Stop();
		break;
	default:
		break;
	}

}

void Stage_one_one::GH_LevelUpdate()
{

	BackgroundPlayer->GH_PlayAlone("Green_Greens.mp3", -1);

}