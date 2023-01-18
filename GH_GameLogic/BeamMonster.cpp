#include "BeamMonster.h"
#include "GameLogicEnum.h"
#include <GH_GameEngineCollision.h>
#include <GH_GameEngineInput.h>
#include <GH_Renderer.h>
#include <GH_GameEngineTime.h>
#include "Normal_Kirby.h"
#include <string>
#include <GH_GameEngineSoundPlayer.h>

float BeamMonster::TimeCheck = 0.2f;
int BeamMonster::Movement = 0;

BeamMonster::BeamMonster()
	: MonsterDebugState_(DEBUGMODE::PLAYMODE)
{
	GH_SetRenderOrder(4);
	MonsterName_ = "BeamMonster";
	
	//MoveMent 를 위해서 내 위치를 받아오기 위한 내부 SetPos
}

BeamMonster::~BeamMonster()
{
}

BeamMonster::BeamMonster(BeamMonster&& _Other) noexcept
{
}

void BeamMonster::GH_LeftRightCheck()
{
	MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
}

std::string BeamMonster::GH_GetDirString()
{
	//내가 오른쪽인 상태면 문자열 "R"을,
	//내가 왼쪽인 상태면 문자열 "L"을 리턴해주는 함수
	switch (dirstate_)
	{
	case GH_MonsterDirectionState::LEFT:
		return "L";
		break;
	case GH_MonsterDirectionState::RIGHT:
		return "R";
		break;
	default:
		GH_GameEngineDebug::Assert();
		break;
	}

	GH_GameEngineDebug::Assert();
	return "";
}


void BeamMonster::GH_Update()
{

	if (false == BeamMonster::GH_GameEngineObjectBase::GH_IsOn())
	{
		return;
	}


	if (MonsterDebugState_ == DEBUGMODE::DEBUGMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		MonsterDebugState_ = DEBUGMODE::PLAYMODE;
	}
	else if (MonsterDebugState_ == DEBUGMODE::PLAYMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		MonsterDebugState_ = DEBUGMODE::DEBUGMODE;
	}

	BeamAttackCollision->GH_SetPivot({ 0, -35 });

	switch (stateType_)
	{
	case GH_MonsterState::Walk:
		GH_Walk();
		break;
	case GH_MonsterState::Damage:
		GH_Damage();
		break;
	case GH_MonsterState::Dead:
		GH_Dead();
		break;
	case GH_MonsterState::GetDrain:
		GH_GetDrain();
		break;
	case GH_MonsterState::AttackReady:
		GH_AttackReady();
		break;
	case GH_MonsterState::Attack:
		GH_Attack();
		break;
	default:
		break;
	}

}

void BeamMonster::GH_Walk()
{
	GH_LeftRightCheck();
	
	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	
	if (dirstate_ == GH_MonsterDirectionState::RIGHT && -200 < (Movement - GH_GetPos().ix()))
	{
		GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 50.0f);
	}
	else if (dirstate_ == GH_MonsterDirectionState::RIGHT && -200 >= (Movement - GH_GetPos().ix()))
	{
		dirstate_ = GH_MonsterDirectionState::LEFT;
	}

	//좌우로 200만큼 움직이게 하게한다

	if (dirstate_ == GH_MonsterDirectionState::LEFT && 0 > (Movement - GH_GetPos().ix()))
	{
		GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 50.0f);
	}
	else if (dirstate_ == GH_MonsterDirectionState::LEFT && 0 <= (Movement - GH_GetPos().ix()))
	{
		dirstate_ = GH_MonsterDirectionState::RIGHT;
	}
	
	if (TimeCheck < 0)
	{
		GH_ChangeState(GH_MonsterState::AttackReady);
	}

}

void BeamMonster::GH_Damage()
{
	GH_LeftRightCheck();

	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	
	//데미지를 받은 상태면 사이즈를 0,0으로 만드는 것으로 모든 데미지 충돌판정을 받지 않는다
	BodyCheckCollision->GH_SetSize({0,0});
	BeamAttackCollision->GH_SetSize({0,0});

	switch (dirstate_)
	{
	case GH_MonsterDirectionState::LEFT:
	{
		GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 100.0f);
	}
	break;
	case GH_MonsterDirectionState::RIGHT:
	{
		GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 100.0f);
	}
		break;
	default:
		break;
	}

	if (TimeCheck < 0)
	{
		TimeCheck = 0.2f;
		GH_ChangeState(GH_MonsterState::Dead);
	}
	
}

void BeamMonster::GH_AttackReady()
{
	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();

	if (TimeCheck < 0)
	{
		GH_ChangeState(GH_MonsterState::Attack);
	}

}

void BeamMonster::GH_GetDrain()
{
	GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 100.0f);
}

void BeamMonster::GH_Dead()
{
	EffectPlayer->GH_PlayAlone("MonsterDead.wav");

	if (StateName_ == "GetDrain")
	{
		GH_SetPos({ -100,-100 });
		BeamMonster::GH_GameEngineObjectBase::GH_Off();
	}
	else if (StateName_ != "GetDrain" && DestroyRender_->GH_IsCurAnimationEnd())
	{
		GH_SetPos({ -100,-100 });
		BeamMonster::GH_GameEngineObjectBase::GH_Off();
	}

}


void BeamMonster::GH_Render()
{

	if (false == BeamMonster::GH_GameEngineObjectBase::GH_IsOn())
	{
		return;
	}


	if (MonsterDebugState_ == DEBUGMODE::DEBUGMODE)
	{
		BodyCheckCollision->GH_DebugRender();
		BeamAttackCollision->GH_DebugRender();
	}
	

	if (stateType_ != GH_MonsterState::Dead)
	{
		MainRender_->GH_AnimationUpdate();
	}
	else
	{
		DestroyRender_->GH_AnimationUpdate();
	}

	
}

void BeamMonster::GH_Attack()
{
	EffectPlayer->GH_PlayAlone("BeamMonsterAttack.wav");


	switch (dirstate_)
	{
	case GH_MonsterDirectionState::LEFT:
		BeamAttackCollision->GH_SetPivot({ -70, -35 });
		break;
	case GH_MonsterDirectionState::RIGHT:
		BeamAttackCollision->GH_SetPivot({ 70, -35 });
		break;
	default:
		break;
	}

	BeamAttackCollision->GH_SetSize({ 200,70 });

	if (MainRender_->GH_IsCurAnimationEnd())
	{
		EffectPlayer->GH_PlayCountReset(1);
		BeamAttackCollision->GH_SetSize({ 0,0 });
		GH_ChangeState(GH_MonsterState::Walk);
	}
}

void BeamMonster::GH_Start()
{
	GH_SetUpdateOrder(100);
	GH_SetRenderOrder(50);
	


	BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTER, GH_CollisionCheckType::RECTANGLE);
	BodyCheckCollision->GH_SetSize({ 70,70 });
	BodyCheckCollision->GH_SetPivot({ 0, -35 });

	BeamAttackCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTERATTACK, GH_CollisionCheckType::RECTANGLE);
	BeamAttackCollision->GH_SetSize({ 0,0 });

	//클래스이름+ 행동 + 좌우 형태로 애니메이션 제작
	MainRender_ = GH_CreateRenderer("Beam_Enemy.bmp");
	MainRender_->GH_CreateAnimation("BeamMonsterWalkR", "Beam_Enemy.bmp", 0, 7, true, 0.17f);
	MainRender_->GH_CreateAnimation("BeamMonsterWalkL", "Beam_Enemy.bmp", 0 + 20, 7 + 20, true, 0.17f);
	MainRender_->GH_CreateAnimation("BeamMonsterDamageR", "Beam_Enemy.bmp", 9, 9, false);
	MainRender_->GH_CreateAnimation("BeamMonsterDamageL","Beam_Enemy.bmp", 9+20, 9+20, false);
	MainRender_->GH_CreateAnimation("BeamMonsterGetDrainR", "Beam_Enemy.bmp", 9, 9, true);
	MainRender_->GH_CreateAnimation("BeamMonsterGetDrainL", "Beam_Enemy.bmp", 9 + 20, 9 + 20, true);
	MainRender_->GH_CreateAnimation("BeamMonsterAttackR", "Beam_Enemy.bmp", 12 , 18 , false);
	MainRender_->GH_CreateAnimation("BeamMonsterAttackL", "Beam_Enemy.bmp", 12 + 20, 18 + 20, false);
	MainRender_->GH_CreateAnimation("BeamMonsterAttackReadyR", "Beam_Enemy.bmp", 10, 11, true, 0.05f);
	MainRender_->GH_CreateAnimation("BeamMonsterAttackReadyL", "Beam_Enemy.bmp", 10 + 20, 11 + 20, true, 0.05f);

	DestroyRender_ = GH_CreateRenderer("Monster_Death.bmp");
	DestroyRender_->GH_CreateAnimation("MonsterDeath", "Monster_Death.bmp", 0, 3, false, 0.05f);

	dirstate_ = GH_MonsterDirectionState::RIGHT;
	GH_ChangeState(GH_MonsterState::Walk);


	EffectPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	if (EffectPlayer == nullptr)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 플레이어 생성이 되지 않았습니다");
	}

	//재생 횟수를 매번 설정 해 주는 PlayCoundReset
	EffectPlayer->GH_PlayCountReset(1);

	GH_SetName("BeamMonster");
}

void BeamMonster::GH_ChangeState(GH_MonsterState _StateType)
{
	stateType_ = _StateType;

	//나의 상태가 들어오면 애니메이션을 재생시킨다
	switch (stateType_)
	{
	case GH_MonsterState::Walk:
		StateName_ = "Walk";
		//5초동안 걸어다니다가 공격을 함
		TimeCheck = 5.0f;
		MainRender_->GH_ChangeAnimation(MonsterName_+ StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Damage:
		StateName_ = "Damage";
		TimeCheck = 0.2f;
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		MainRender_->GH_ChangeAnimation(MonsterName_+ StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::GetDrain:
		StateName_ = "GetDrain";
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Dead:
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		DestroyRender_->GH_ChangeAnimation("MonsterDeath", true);
		break;
	case GH_MonsterState::AttackReady:
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		StateName_ = "AttackReady";
		//1초동안 공격준비를함
		TimeCheck = 1.0f;
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Attack:
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		StateName_ = "Attack";
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}

}

