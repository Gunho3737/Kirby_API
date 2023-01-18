#include "WeakMonster.h"
#include "GameLogicEnum.h"
#include <GH_GameEngineCollision.h>
#include <GH_GameEngineInput.h>
#include <GH_Renderer.h>
#include <GH_GameEngineTime.h>
#include "Normal_Kirby.h"
#include <string>
#include <GH_GameEngineSoundPlayer.h>

float WeakMonster::TimeCheck = 0.2f;
int WeakMonster::Movement = 0;

WeakMonster::WeakMonster()
	: MonsterDebugState_(DEBUGMODE::PLAYMODE)
{
	GH_SetRenderOrder(4);
	MonsterName_ = "WeakMonster";

}

WeakMonster::~WeakMonster()
{
}

WeakMonster::WeakMonster(WeakMonster&& _Other) noexcept
{
}

void WeakMonster::GH_LeftRightCheck()
{
	MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
}

std::string WeakMonster::GH_GetDirString()
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


void WeakMonster::GH_Update()
{
	if (false == WeakMonster::GH_GameEngineObjectBase::GH_IsOn())
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
	default:
		break;
	}

}

void WeakMonster::GH_Walk()
{
	GH_LeftRightCheck();
	
	

	//좌우로 200만큼 움직이게 하게한다
	if (dirstate_ == GH_MonsterDirectionState::LEFT && 200 > (Movement - GH_GetPos().ix()))
	{
		GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 50.0f);
	}
	else if (dirstate_ == GH_MonsterDirectionState::LEFT && 200 <= (Movement - GH_GetPos().ix()))
	{
		dirstate_ = GH_MonsterDirectionState::RIGHT;
	}
	
	if (dirstate_ == GH_MonsterDirectionState::RIGHT && 0 < (Movement - GH_GetPos().ix()))
	{
		GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 50.0f);
	}
	else if (dirstate_ == GH_MonsterDirectionState::RIGHT && 0 >= (Movement - GH_GetPos().ix()))
	{
		dirstate_ = GH_MonsterDirectionState::LEFT;
	}

}

void WeakMonster::GH_Damage()
{
	GH_LeftRightCheck();

	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	
	//데미지를 받은 상태면 사이즈를 0,0으로 만드는 것으로 모든 데미지 충돌판정을 받지 않는다
	BodyCheckCollision->GH_SetSize({0,0});

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

void WeakMonster::GH_GetDrain()
{
	GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 100.0f);
}

void WeakMonster::GH_Dead()
{
	EffectPlayer->GH_PlayAlone("MonsterDead.wav");

	if (StateName_ == "GetDrain")
	{
		GH_SetPos({ -100,-100 });
		WeakMonster::GH_GameEngineObjectBase::GH_Off();
	}
	else if (StateName_ != "GetDrain" && DestroyRender_->GH_IsCurAnimationEnd())
	{
		GH_SetPos({ -100,-100 });
		WeakMonster::GH_GameEngineObjectBase::GH_Off();
	}
	
	

}


void WeakMonster::GH_Render()
{
	if (false == WeakMonster::GH_GameEngineObjectBase::GH_IsOn())
	{
		return;
	}

	if (MonsterDebugState_ == DEBUGMODE::DEBUGMODE)
	{
		BodyCheckCollision->GH_DebugRender();
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

void WeakMonster::GH_Start()
{
	GH_SetUpdateOrder(100);
	GH_SetRenderOrder(50);

	BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTER, GH_CollisionCheckType::RECTANGLE);
	BodyCheckCollision->GH_SetSize({ 70,70 });
	BodyCheckCollision->GH_SetPivot({ 0, -35 });

	MainRender_ = GH_CreateRenderer("Waddle_Di.bmp");
	MainRender_->GH_CreateAnimation("WeakMonsterWalkR", "Waddle_Di.bmp", 0, 7, true, 0.17f);
	MainRender_->GH_CreateAnimation("WeakMonsterWalkL", "Waddle_Di.bmp", 0 + 20, 7 + 20, true, 0.17f);
	MainRender_->GH_CreateAnimation("WeakMonsterDamageR", "Waddle_Di.bmp", 9, 9, true);
	MainRender_->GH_CreateAnimation("WeakMonsterDamageL", "Waddle_Di.bmp", 9+20, 9+20, true);
	MainRender_->GH_CreateAnimation("WeakMonsterGetDrainR", "Waddle_Di.bmp", 9, 9, true);
	MainRender_->GH_CreateAnimation("WeakMonsterGetDrainL", "Waddle_Di.bmp", 9 + 20, 9 + 20, true);

	DestroyRender_ = GH_CreateRenderer("Monster_Death.bmp");
	DestroyRender_->GH_CreateAnimation("MonsterDeath", "Monster_Death.bmp", 0, 3, false, 0.05f);

	dirstate_ = GH_MonsterDirectionState::LEFT;
	

	EffectPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	if (EffectPlayer == nullptr)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 플레이어 생성이 되지 않았습니다");
	}

	//재생 횟수를 매번 설정 해 주는 PlayCoundReset
	EffectPlayer->GH_PlayCountReset(1);

	GH_ChangeState(GH_MonsterState::Walk);

	GH_SetName("WeakMonster");
}

void WeakMonster::GH_ChangeState(GH_MonsterState _StateType)
{
	stateType_ = _StateType;

	//나의 상태가 들어오면 애니메이션을 재생시킨다
	switch (stateType_)
	{
	case GH_MonsterState::Walk:
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		StateName_ = "Walk";
		MainRender_->GH_ChangeAnimation(MonsterName_+ StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Damage:
		StateName_ = "Damage";
		MainRender_->GH_ChangeAnimation(MonsterName_+ StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::GetDrain:
		StateName_ = "GetDrain";
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Dead:
		DestroyRender_->GH_ChangeAnimation("MonsterDeath", true);
		break;
	default:
		break;
	}

}

