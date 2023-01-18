#include "CutterMonster.h"
#include "GameLogicEnum.h"
#include <GH_GameEngineCollision.h>
#include <GH_GameEngineInput.h>
#include <GH_Renderer.h>
#include <GH_GameEngineTime.h>
#include "Normal_Kirby.h"
#include <string>
#include "Bullet.h"
#include <GH_Level.h>
#include <GH_GameEngineSoundPlayer.h>

float CutterMonster::TimeCheck = 0.2f;
int CutterMonster::Movement = 0;

CutterMonster::CutterMonster()
	: MonsterDebugState_(DEBUGMODE::PLAYMODE)
{
	GH_SetRenderOrder(4);
	
	MonsterName_ = "CutterMonster";
	
	//MoveMent �� ���ؼ� �� ��ġ�� �޾ƿ��� ���� ���� SetPos, ���ϴ� ���� ��ġ��Ű��
	ShootMoveActive = false;
}

CutterMonster::~CutterMonster()
{
}

CutterMonster::CutterMonster(CutterMonster&& _Other) noexcept
{
}

void CutterMonster::GH_LeftRightCheck()
{
	MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
}

std::string CutterMonster::GH_GetDirString()
{
	//���� �������� ���¸� ���ڿ� "R"��,
	//���� ������ ���¸� ���ڿ� "L"�� �������ִ� �Լ�
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


void CutterMonster::GH_Update()
{
	if (false == CutterMonster::GH_GameEngineObjectBase::GH_IsOn())
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
	case GH_MonsterState::AttackReady:
		GH_AttackReady();
		break;
	case GH_MonsterState::Attack:
		GH_Attack();
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
	case GH_MonsterState::Jump:
		GH_Jump();
		break;
	default:
		break;
	}

}

void CutterMonster::GH_AttackReady()
{
	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();

	if (TimeCheck < 0)
	{
		TimeCheck = 5.0f;
		GH_ChangeState(GH_MonsterState::Jump);
	}

}

void CutterMonster::GH_Attack()
{
	if (false == ShootMoveActive)
	{
		Bullet* BulletPtr = CutterMonster::GH_Actor::GH_GetLevel()->GH_CreateActor<Bullet>();

		if (dirstate_ == GH_MonsterDirectionState::LEFT)
		{
			BulletPtr->BulletDir_ = BulletDirectionState::LEFT;
		}
		else
		{
			BulletPtr->BulletDir_ = BulletDirectionState::RIGHT;
		}

		BulletPtr->GH_ChangeState(GH_BulletState::MonsterCutter);
		
		BulletPtr->GH_SetPos(GH_GetPos());
		ShootMoveActive = true;
	}


	if ((Movement - GH_GetPos().iy()) >= 0)
	{
		GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
	}
	else
	{
		ShootMoveActive = false;
		GH_ChangeState(GH_MonsterState::AttackReady);
	}
}

void CutterMonster::GH_Damage()
{
	GH_LeftRightCheck();

	TimeCheck -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	
	//�������� ���� ���¸� ����� 0,0���� ����� ������ ��� ������ �浹������ ���� �ʴ´�
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

void CutterMonster::GH_GetDrain()
{
	GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 100.0f);
}

void CutterMonster::GH_Dead()
{
	EffectPlayer->GH_PlayAlone("MonsterDead.wav");

	if (StateName_ == "GetDrain")
	{
		GH_SetPos({ -100,-100 });
		CutterMonster::GH_GameEngineObjectBase::GH_Off();
	}
	else if (StateName_ != "GetDrain" && DestroyRender_->GH_IsCurAnimationEnd())
	{
		GH_SetPos({ -100,-100 });
		CutterMonster::GH_GameEngineObjectBase::GH_Off();
	}


}


void CutterMonster::GH_Render()
{
	if (false == CutterMonster::GH_GameEngineObjectBase::GH_IsOn())
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

void CutterMonster::GH_Jump()
{
	if ((Movement - GH_GetPos().iy()) < 200)
	{
		GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
	}
	else
	{
		GH_ChangeState(GH_MonsterState::Attack);
	}

	

	
}

void CutterMonster::GH_Start()
{
	GH_SetUpdateOrder(100);
	GH_SetRenderOrder(50);
	


	BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTER, GH_CollisionCheckType::RECTANGLE);
	BodyCheckCollision->GH_SetSize({ 70,70 });
	BodyCheckCollision->GH_SetPivot({ 0, -35 });

	//Ŭ�����̸�+ �ൿ + �¿� ���·� �ִϸ��̼� ����
	MainRender_ = GH_CreateRenderer("Cutter_Enemy.bmp");
	MainRender_->GH_CreateAnimation(MonsterName_+"AttackReadyR", "Cutter_Enemy.bmp", 0, 2, true, 0.3f);
	MainRender_->GH_CreateAnimation(MonsterName_+"AttackReadyL", "Cutter_Enemy.bmp", 0 + 10, 2 + 10, true, 0.3f);
	MainRender_->GH_CreateAnimation(MonsterName_+"DamageR", "Cutter_Enemy.bmp", 6, 6, true);
	MainRender_->GH_CreateAnimation(MonsterName_+"DamageL", "Cutter_Enemy.bmp", 6+10, 6+10, true);
	MainRender_->GH_CreateAnimation(MonsterName_+"GetDrainR", "Cutter_Enemy.bmp", 6, 6, true);
	MainRender_->GH_CreateAnimation(MonsterName_+"GetDrainL", "Cutter_Enemy.bmp", 6 + 10, 6 + 10, true);
	MainRender_->GH_CreateAnimation(MonsterName_ + "JumpR", "Cutter_Enemy.bmp", 3, 3, false);
	MainRender_->GH_CreateAnimation(MonsterName_ + "JumpL", "Cutter_Enemy.bmp", 3+10, 3+10, false);
	MainRender_->GH_CreateAnimation(MonsterName_ + "AttackR", "Cutter_Enemy.bmp", 4, 5, false);
	MainRender_->GH_CreateAnimation(MonsterName_ + "AttackL", "Cutter_Enemy.bmp", 4+10, 5+10, false);

	DestroyRender_ = GH_CreateRenderer("Monster_Death.bmp");
	DestroyRender_->GH_CreateAnimation("MonsterDeath", "Monster_Death.bmp", 0, 3, false, 0.05f);

	dirstate_ = GH_MonsterDirectionState::LEFT;


	EffectPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	if (EffectPlayer == nullptr)
	{
		GH_GameEngineDebug::MessageBoxError("���� �÷��̾� ������ ���� �ʾҽ��ϴ�");
	}

	//��� Ƚ���� �Ź� ���� �� �ִ� PlayCoundReset
	EffectPlayer->GH_PlayCountReset(1);

	GH_ChangeState(GH_MonsterState::AttackReady);

	GH_SetName("CutterMonster");
}

void CutterMonster::GH_ChangeState(GH_MonsterState _StateType)
{
	stateType_ = _StateType;

	//���� ���°� ������ �ִϸ��̼��� �����Ų��
	switch (stateType_)
	{
	case GH_MonsterState::AttackReady:
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		StateName_ = "AttackReady";
		TimeCheck = 5.0f;
		MainRender_->GH_ChangeAnimation(MonsterName_+ StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Attack:
		StateName_ = "Attack";
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Damage:
		StateName_ = "Damage";
		TimeCheck = 0.2f;
		MainRender_->GH_ChangeAnimation(MonsterName_+ StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::GetDrain:
		StateName_ = "GetDrain";
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Jump:
		StateName_ = "Jump";
		MainRender_->GH_ChangeAnimation(MonsterName_ + StateName_ + GH_GetDirString());
		break;
	case GH_MonsterState::Dead:
		DestroyRender_->GH_ChangeAnimation("MonsterDeath", true);
		break;
	default:
		break;
	}

}

