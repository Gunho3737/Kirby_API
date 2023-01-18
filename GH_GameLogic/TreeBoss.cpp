#include "TreeBoss.h"
#include "GameLogicEnum.h"
#include <GH_GameEngineCollision.h>
#include <GH_GameEngineInput.h>
#include <GH_Renderer.h>
#include <GH_GameEngineTime.h>
#include "Normal_Kirby.h"
#include <string>
#include "Bullet.h"
#include <GH_Level.h>
#include "Stage_one_one.h"
#include <GH_GameEngineSoundPlayer.h>
#include <GH_LevelManager.h>

float TreeBoss::TimeCheck = 2.0f;
float TreeBoss::AttackTimeCheck = 5.0f;


TreeBoss::TreeBoss()
	: MonsterDebugState_(DEBUGMODE::PLAYMODE), BreathCount(2)
{
	GH_SetRenderOrder(4);
	
	MonsterName_ = "TreeBoss";

}

TreeBoss::~TreeBoss()
{
}

TreeBoss::TreeBoss(TreeBoss&& _Other) noexcept
{
}


void TreeBoss::GH_Update()
{
	if (MonsterDebugState_ == DEBUGMODE::DEBUGMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		MonsterDebugState_ = DEBUGMODE::PLAYMODE;
	}
	else if (MonsterDebugState_ == DEBUGMODE::PLAYMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		MonsterDebugState_ = DEBUGMODE::DEBUGMODE;
	}


	switch (stateType_)
	{
	case GH_MonsterState::AttackReady:
		GH_AttackReady();
		break;
	case GH_MonsterState::Idle:
		GH_Idle();
		break;
	case GH_MonsterState::Attack:
		GH_Attack();
		break;
	case GH_MonsterState::Breath:
		GH_Breath();
		break;
	case GH_MonsterState::Damage:
		GH_Damage();
		break;
	case GH_MonsterState::Dead:
		GH_Dead();
		break;
	default:
		break;
	}

}

void TreeBoss::GH_AttackReady()
{
	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	AttackTimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();

	if (AttackTimeCheck < 0)
	{
		AttackTimeCheck = 5.0f;
		GH_ChangeState(GH_MonsterState::Breath);
	}

	if (TimeCheck < 0)
	{
		GH_ChangeState(GH_MonsterState::Idle);
	}

	
	
}

void TreeBoss::GH_Idle()
{
	AttackTimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	if (MainRender_->GH_IsCurAnimationEnd())
	{
		TimeCheck = 2.0f;
		GH_ChangeState(GH_MonsterState::AttackReady);
	}
}

void TreeBoss::GH_Attack()
{
	Bullet* BulletPtr = TreeBoss::GH_Actor::GH_GetLevel()->GH_CreateActor<Bullet>();

	BulletPtr->GH_ChangeState(GH_BulletState::Apple);

	BulletPtr->GH_SetPos({Stage_one_one::NormalKirbyPtr->GH_GetPos().x,100 });

	BulletPtr->BulletDir_ = BulletDirectionState::LEFT;

	GH_ChangeState(GH_MonsterState::AttackReady);
}

void TreeBoss::GH_Breath()
{
	EffectPlayer->GH_PlayOverLap("BossYawn.wav");


	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();

	if (TimeCheck < 0)
	{
		

		if (MainRender_->GH_IsCurAnimationEnd())
		{
			EffectPlayer->GH_PlayCountReset(1);
			MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_, true);
			Bullet* BulletPtr = TreeBoss::GH_Actor::GH_GetLevel()->GH_CreateActor<Bullet>();
			BulletPtr->GH_ChangeState(GH_BulletState::Breath);
			BulletPtr->GH_SetPos({550,450});
			BulletPtr->BulletDir_ = BulletDirectionState::LEFT;
			BreathCount -= 1;
			if (0 >= BreathCount)
			{
				BreathCount = 2;
				TimeCheck = 2.0f;
				GH_ChangeState(GH_MonsterState::Attack);
			}
			
		}

	}
}

void TreeBoss::GH_Damage()
{
	EffectPlayer->GH_PlayOverLap("MonsterDead.wav");

	if (0 >= HP)
	{
		GH_ChangeState(GH_MonsterState::Dead);
	}

	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	AttackTimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	
	//데미지를 받은 상태면 사이즈를 0,0으로 만드는 것으로 모든 데미지 충돌판정을 받지 않는다
	BodyCheckCollision->GH_SetSize({0,0});
	if (TimeCheck < 0)
	{
		EffectPlayer->GH_PlayCountReset();
		TimeCheck = 2.0f;
		GH_ChangeState(GH_MonsterState::AttackReady);
	}
	
}



void TreeBoss::GH_Dead()
{
	Stage_one_one::StagePtr->BackgroundPlayer->GH_Stop();
	EffectPlayer->GH_PlayAlone("BossDead.wav");

	if (false == EffectPlayer->GH_IsPlay())
	{
		//GH_GetLevel()->ChangeStage();
		//GH_LevelManager::GH_GetInst().GH_ChangeLevel("EndLevel");
		if (false == KirbyDanceActive)
		{
			
			Stage_one_one::NormalKirbyPtr->GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DanceReady);
			KirbyDanceActive = true;
		}
		
	
	}
}



void TreeBoss::GH_Render()
{
	if (MonsterDebugState_ == DEBUGMODE::DEBUGMODE)
	{
		BodyCheckCollision->GH_DebugRender();
	}
	


	MainRender_->GH_AnimationUpdate();

	if (stateType_ == GH_MonsterState::Dead)
	{
		EffectRender_->GH_AnimationUpdate();
	}
	
}

void TreeBoss::GH_Start()
{
	GH_SetUpdateOrder(100);
	GH_SetRenderOrder(50);

	HP = 9;

	BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTER, GH_CollisionCheckType::RECTANGLE);
	BodyCheckCollision->GH_SetSize({ 70,300 });
	BodyCheckCollision->GH_SetPivot({ 0, -70 });

	//클래스이름+ 행동 + 좌우 형태로 애니메이션 제작
	MainRender_ = GH_CreateRenderer("Tree_Boss.bmp");
	MainRender_->GH_CreateAnimation(MonsterName_+"AttackReady", "Tree_Boss.bmp", 0, 0, false);
	MainRender_->GH_CreateAnimation(MonsterName_+"Damage", "Tree_Boss.bmp", 10, 10, false);
	MainRender_->GH_CreateAnimation(MonsterName_ + "Idle", "Tree_Boss.bmp", 1, 6, false, 0.04f);
	MainRender_->GH_CreateAnimation(MonsterName_ + "Dead", "Tree_Boss.bmp", 11, 11, false);
	MainRender_->GH_CreateAnimation(MonsterName_ + "Breath", "Tree_Boss.bmp", 7, 9, false);

	EffectRender_ = GH_CreateRenderer("Tree_Damage_Effect.bmp");
	EffectRender_->GH_CreateAnimation("TreeBossDamage", "Tree_Damage_Effect.bmp", 0, 5, false, 0.03f);

	EffectPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	if (EffectPlayer == nullptr)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 플레이어 생성이 되지 않았습니다");
	}

	//재생 횟수를 매번 설정 해 주는 PlayCoundReset
	EffectPlayer->GH_PlayCountReset(1);

	GH_ChangeState(GH_MonsterState::AttackReady);

	GH_SetName("TreeBoss");
}

void TreeBoss::GH_ChangeState(GH_MonsterState _StateType)
{
	stateType_ = _StateType;

	//나의 상태가 들어오면 애니메이션을 재생시킨다
	switch (stateType_)
	{
	case GH_MonsterState::AttackReady:
		BodyCheckCollision->GH_SetPivot({ 0, -70 });
		BodyCheckCollision->GH_SetSize({ 70, 300 });
		StateName_ = "AttackReady";
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_);
		break;
	case GH_MonsterState::Idle:
		StateName_ = "Idle";
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_);
		break;
	case GH_MonsterState::Damage:
		HP -= 1;
		BodyCheckCollision->GH_SetPivot({ 0, -1024 });
		TimeCheck = 0.7f;
		StateName_ = "Damage";
		MainRender_->GH_ChangeAnimation(MonsterName_+ StateName_);
		break;
	case GH_MonsterState::Attack:
		MainRender_->GH_ChangeAnimation(MonsterName_ + "AttackReady");
		break;
	case GH_MonsterState::Breath:
		TimeCheck = 0.3f;
		BodyCheckCollision->GH_SetPivot({ 0, -1024 });
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		StateName_ = "Breath";
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_);
		break;
	case GH_MonsterState::Dead:
		BodyCheckCollision->GH_SetPivot({ 0, -1024 });
		StateName_ = "Dead";
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_);
		EffectRender_->GH_ChangeAnimation("TreeBossDamage");
		break;
	default:
		break;
	}

}

