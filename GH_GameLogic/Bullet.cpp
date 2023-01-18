#include "Bullet.h"
#include "GameLogicEnum.h"
#include <GH_GameEngineCollision.h>
#include <GH_GameEngineInput.h>
#include <GH_Renderer.h>
#include <GH_GameEngineTime.h>
#include "Normal_Kirby.h"
#include <string>
#include "One_One_Back.h"
#include "WeakMonster.h"
#include "BeamMonster.h"
#include "CutterMonster.h"
#include "TreeBoss.h"
#include <GH_GameEngineSoundPlayer.h>

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
}

Bullet::Bullet(Bullet&& _Other) noexcept
{
}

void Bullet::CheckBulletType()
{
	if ("Apple" == GH_GetName())
	{
		return;
	}

	if ("Breath" == GH_GetName())
	{
		return;
	}


	DestroyRender_->GH_AnimationUpdate();
}

void Bullet::GH_Start()
{
	GH_SetUpdateOrder(99);
	GH_SetRenderOrder(49);

	BulletDebugState_ = DEBUGMODE::PLAYMODE;
	MainRender_ = GH_CreateRenderer("Star.bmp");
	MainRender_->GH_CreateAnimation("StarMove", "Star.bmp", 0, 3, true,0.07f);

	CutterRender_ = GH_CreateRenderer("Cutter_Att.bmp");
	CutterRender_->GH_CreateAnimation("CutterMove", "Cutter_Att.bmp", 0, 3, true, 0.02f);

	MonsterCutterRender_ = GH_CreateRenderer("Cutter_Enemy.bmp");
	MonsterCutterRender_->GH_CreateAnimation("MonsterCutterMove", "Cutter_Enemy.bmp", 8, 9, true, 0.02f);

	AppleRender_ = GH_CreateRenderer("Apple.bmp");
	AppleRender_->GH_CreateAnimation("AppleSpin", "Apple.bmp", 0, 7, true, 0.05f);

	BreathRender_ = GH_CreateRenderer("Breath.bmp");
	BreathRender_->GH_CreateAnimation("BreathSpin","Breath.bmp", 0, 3, true, 0.05f);

	DestroyRender_ = GH_CreateRenderer("Star_Impact.bmp");
	DestroyRender_->GH_CreateAnimation("DestroyR", "Star_Impact.bmp", 0, 4, false, 0.02f);
	DestroyRender_->GH_CreateAnimation("DestroyL", "Star_Impact.bmp", 0+5, 4+5, false, 0.02f);

	KirbyBreathRender_ = GH_CreateRenderer("Kirby_Breath.bmp");
	KirbyBreathRender_->GH_CreateAnimation("KirbyBreathR", "Kirby_Breath.bmp", 0, 5, false, 0.07f);
	KirbyBreathRender_->GH_CreateAnimation("KirbyBreathL", "Kirby_Breath.bmp", 0+10, 5+10, false, 0.07f);

	BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::BULLET, GH_CollisionCheckType::RECTANGLE);
	WallCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::BULLET, GH_CollisionCheckType::POINT);
	GroundCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::BULLET, GH_CollisionCheckType::POINT);


	switch (StateType_)
	{
	case GH_BulletState::Star:
		BodyCheckCollision->GH_SetSize({ 70,70 });
		BodyCheckCollision->GH_SetPivot({ 0,-30 });
		WallCheckCollision->GH_SetPivot({ -35, -35 });
		WallCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		break;
	case GH_BulletState::Cutter:
		BodyCheckCollision->GH_SetSize({100, 70});
		BodyCheckCollision->GH_SetPivot({ 0,-30 });
		WallCheckCollision->GH_SetPivot({ -100, -35 });
		WallCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		break;
	case GH_BulletState::MonsterCutter:
		BodyCheckCollision->GH_SetSize({ 70,70 });
		BodyCheckCollision->GH_SetPivot({ 0,-30 });
		WallCheckCollision->GH_SetPivot({ -35, -35 });
		WallCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		break;
	case GH_BulletState::Apple:
		BodyCheckCollision->GH_SetSize({ 80,80 });
		BodyCheckCollision->GH_SetPivot({ 0,30 });
		GroundCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		break;
	case GH_BulletState::Breath:
		BodyCheckCollision->GH_SetSize({ 70,70 });
		BodyCheckCollision->GH_SetPivot({ 0,30 });
		WallCheckCollision->GH_SetPivot({ -35, -35 });
		WallCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		break;
	case GH_BulletState::KirbyBreath:
		BodyCheckCollision = nullptr;
		WallCheckCollision = nullptr;
		break;
	case GH_BulletState::Dead:
		break;
	default:
		break;
	}

	EffectPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	if (EffectPlayer == nullptr)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 플레이어 생성이 되지 않았습니다");
	}

	//재생 횟수를 매번 설정 해 주는 PlayCoundReset
	EffectPlayer->GH_PlayCountReset(1);

}

void Bullet::GH_Update()
{
	if (BulletDebugState_ == DEBUGMODE::DEBUGMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		BulletDebugState_ = DEBUGMODE::PLAYMODE;
	}
	else if (BulletDebugState_ == DEBUGMODE::PLAYMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		BulletDebugState_ = DEBUGMODE::DEBUGMODE;
	}


	switch (StateType_)
	{
	case GH_BulletState::Star:
		Star();
		break;
	case GH_BulletState::Cutter:
		Cutter();
		break;
	case GH_BulletState::MonsterCutter:
		MonsterCutter();
		break;
	case GH_BulletState::Apple:
		Apple();
		break;
	case GH_BulletState::Breath:
		Breath();
		break;
	case GH_BulletState::GetDrain:
		GetDrain();
		break;
	case GH_BulletState::KirbyBreath:
		KirbyBreath();
		break;
	case GH_BulletState::Dead:
		Dead();
		break;
	default:
		break;
	}
}

void Bullet::MonsterCollisionCheck()
{
	GH_GameEngineCollision* Collision = BodyCheckCollision->GH_CollisionGroupCheckOne(GAMECOLLISIONGROUP::MONSTER);


	if (nullptr != Collision)
	{
		std::string CheckMonster = Collision->GH_GetActor()->GH_GetName();

		if (CheckMonster == "WeakMonster")
		{
			WeakMonster* WeakMonsterPtr = Collision->GH_GetActorConvert<WeakMonster>();
			WeakMonsterPtr->GH_ChangeState(GH_MonsterState::Damage);
		}

		if (CheckMonster == "BeamMonster")
		{
			BeamMonster* BeamMonsterPtr = Collision->GH_GetActorConvert<BeamMonster>();
			BeamMonsterPtr->GH_ChangeState(GH_MonsterState::Damage);
		}

		if (CheckMonster == "CutterMonster")
		{
			CutterMonster* CutterMonsterPtr = Collision->GH_GetActorConvert<CutterMonster>();
			CutterMonsterPtr->GH_ChangeState(GH_MonsterState::Damage);
		}

		if (CheckMonster == "TreeBoss")
		{
			TreeBoss* TreeMonsterPtr = Collision->GH_GetActorConvert<TreeBoss>();
			TreeMonsterPtr->GH_ChangeState(GH_MonsterState::Damage);
		}


		GH_ChangeState(GH_BulletState::Dead);
	}
}

void Bullet::KirbyCollisionCheck()
{
	GH_GameEngineCollision* Collision = BodyCheckCollision->GH_CollisionGroupCheckOne(GAMECOLLISIONGROUP::PLAYER);

	if (nullptr != Collision)
	{
		GH_ChangeState(GH_BulletState::Dead);
	}
}

void Bullet::Star()
{
	MonsterCollisionCheck();

	if (BulletDir_ == BulletDirectionState::LEFT)
	{
		WallCheckCollision->GH_SetPivot({ -35, -35 });
		GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 600.0f);
	}
	else if (BulletDir_ == BulletDirectionState::RIGHT)
	{
		WallCheckCollision->GH_SetPivot({ 35, -35 });
		GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 600.0f);
		
	}

	if (true == WallCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		GH_ChangeState(GH_BulletState::Dead);
	}
}

void Bullet::Cutter()
{
	EffectPlayer->GH_PlayAlone("CutterMonsterAttack.wav", 0);
	EffectPlayer->GH_PlayCountReset(1);
	MonsterCollisionCheck();
	if (true == WallCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		GH_ChangeState(GH_BulletState::Dead);
	}

	switch (BulletDir_)
	{
	case BulletDirectionState::LEFT:
	{
		CutterDir_ += GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 900.0f;
		WallCheckCollision->GH_SetPivot({-40, -35 });
		GH_SetMove(CutterDir_ * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime());
		//점점 날아가는 속력이 느려지고, 나중에는 반대방향으로 가게된다

		if (0 <= CutterDir_.x)
		{
			WallCheckCollision->GH_SetPivot({ 40, -35 });
		}
	}
		break;
	case BulletDirectionState::RIGHT:
	{
		CutterDir_ += GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 900.0f;
		WallCheckCollision->GH_SetPivot({ 40, -35 });
		GH_SetMove(CutterDir_ * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime());
		//점점 날아가는 속력이 느려지고, 나중에는 반대방향으로 가게된다

		if (0 >= CutterDir_.x)
		{
			WallCheckCollision->GH_SetPivot({ -40, -35 });
		}
	}
		break;
	default:
		break;
	}
	
}

void Bullet::Apple()
{
	EffectPlayer->GH_PlayAlone("AppleDrop.wav", 0);

	if (true == GroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		GH_ChangeState(GH_BulletState::Dead);
	}

	GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
}

void Bullet::Breath()
{
	if (true == GroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		GH_ChangeState(GH_BulletState::Dead);
	}

	GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 400.0f);
}

void Bullet::MonsterCutter()
{


	if (true == WallCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		GH_ChangeState(GH_BulletState::Dead);
	}

	switch (BulletDir_)
	{
	case BulletDirectionState::LEFT:
	{
		CutterDir_ += GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 900.0f;
		WallCheckCollision->GH_SetPivot({ -40, -35 });
		GH_SetMove(CutterDir_ * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime());
		//점점 날아가는 속력이 느려지고, 나중에는 반대방향으로 가게된다

		if (0 <= CutterDir_.x)
		{
			WallCheckCollision->GH_SetPivot({ 40, -35 });
		}
	}
	break;
	case BulletDirectionState::RIGHT:
	{
		CutterDir_ += GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 900.0f;
		WallCheckCollision->GH_SetPivot({ 40, -35 });
		GH_SetMove(CutterDir_ * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime());
		//점점 날아가는 속력이 느려지고, 나중에는 반대방향으로 가게된다

		if (0 >= CutterDir_.x)
		{
			WallCheckCollision->GH_SetPivot({ -40, -35 });
		}
	}
	break;
	default:
		break;
	}


}

void Bullet::GetDrain()
{
	GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 100.0f);
}

void Bullet::KirbyBreath()
{
	if (BulletDir_ == BulletDirectionState::RIGHT)
	{
		GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 400.0f);
	}
	else if (BulletDir_ == BulletDirectionState::LEFT)
	{
		GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 400.0f);
	}

	if (KirbyBreathRender_->GH_IsCurAnimationEnd())
	{
		GH_ChangeState(GH_BulletState::Dead);
	}
}

void Bullet::Dead()
{
	EffectPlayer->GH_Stop();

	//사과와 보스의 브레스는 애니메이션이 없으므로 바로 삭제
	if ("Apple" == GH_GetName())
	{
		Bullet::GH_GameEngineObjectBase::GH_Death();
	}

	if ("Breath" == GH_GetName())
	{
		Bullet::GH_GameEngineObjectBase::GH_Death();
	}

	if ("KirbyBreath" == GH_GetName())
	{
		Bullet::GH_GameEngineObjectBase::GH_Death();
	}

	//그외는 파괴 애니메이션을 재생하고 삭제
	if (DestroyRender_->GH_IsCurAnimationEnd())
	{
		Bullet::GH_GameEngineObjectBase::GH_Death();
	}

}

void Bullet::GH_Render()
{
	if (BulletDebugState_ == DEBUGMODE::DEBUGMODE)
	{
		BodyCheckCollision->GH_DebugRender();
		WallCheckCollision->GH_DebugRender();
	}
	
	switch (StateType_)
	{
	case GH_BulletState::Star:
		MainRender_->GH_AnimationUpdate();
		break;
	case GH_BulletState::Cutter:
		CutterRender_->GH_AnimationUpdate();
		break;
	case GH_BulletState::Apple:
		AppleRender_->GH_AnimationUpdate();
		break;
	case GH_BulletState::Breath:
		BreathRender_->GH_AnimationUpdate();
		break;
	case GH_BulletState::MonsterCutter:
		MonsterCutterRender_->GH_AnimationUpdate();
		break;
	case GH_BulletState::GetDrain:
		AppleRender_->GH_AnimationUpdate();//현재는 흡수가능한 총알이 사과뿐이다. 나중에 더 추가할거면 switch로 추가
		break;
	case GH_BulletState::KirbyBreath:
		KirbyBreathRender_->GH_AnimationUpdate();
		break;
	case GH_BulletState::Dead:
		CheckBulletType();
		break;
	default:
		break;
	}
	
}


void Bullet::GH_ChangeState(GH_BulletState _StateType)
{
	StateType_ = _StateType;

	//나의 상태가 들어오면 애니메이션을 재생시킨다
	switch (StateType_)
	{
	case GH_BulletState::Star:
		GH_SetName("Star");
		MainRender_->GH_ChangeAnimation("StarMove");
		break;
	case GH_BulletState::Cutter:
		GH_SetName("Cutter");
		switch (BulletDir_)
		{
		case BulletDirectionState::LEFT:
			CutterDir_ = GH_float4::LEFT * 800.0f;
			break;
		case BulletDirectionState::RIGHT:
			CutterDir_ = GH_float4::RIGHT * 800.0f;
			break;
		default:
			break;
		}
		CutterRender_->GH_ChangeAnimation("CutterMove");
		break;
	case GH_BulletState::MonsterCutter:
		GH_SetName("MonsterCutter");
		BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTERATTACK, GH_CollisionCheckType::RECTANGLE);
		switch (BulletDir_)
		{
		case BulletDirectionState::LEFT:
			CutterDir_ = GH_float4::LEFT * 800.0f;
			break;
		case BulletDirectionState::RIGHT:
			CutterDir_ = GH_float4::RIGHT * 800.0f;
			break;
		default:
			break;
		}
		MonsterCutterRender_->GH_ChangeAnimation("MonsterCutterMove");
		break;
	case GH_BulletState::Apple:
		BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTER, GH_CollisionCheckType::RECTANGLE);
		GH_SetName("Apple");
		AppleRender_->GH_ChangeAnimation("AppleSpin");
		break;
	case GH_BulletState::Breath:
		BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTERATTACK, GH_CollisionCheckType::RECTANGLE);
		GH_SetName("Breath");
		BreathRender_->GH_ChangeAnimation("BreathSpin");
		break;
	case GH_BulletState::GetDrain:
		BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MONSTER, GH_CollisionCheckType::RECTANGLE);
		GH_SetName("Apple");
		AppleRender_->GH_ChangeAnimation("AppleSpin");
		break;
	case GH_BulletState::KirbyBreath:
		GH_SetName("KirbyBreath");
		if (BulletDir_ == BulletDirectionState::RIGHT)
		{
			KirbyBreathRender_->GH_ChangeAnimation("KirbyBreathR");
		}
		else if (BulletDir_ == BulletDirectionState::LEFT)
		{
			KirbyBreathRender_->GH_ChangeAnimation("KirbyBreathL");
		}
		break;
	case GH_BulletState::Dead:
		if (BulletDir_ == BulletDirectionState::RIGHT)
		{
			DestroyRender_->GH_ChangeAnimation(("DestroyR"));
		}
		else if (BulletDir_ == BulletDirectionState::LEFT)
		{
			DestroyRender_->GH_ChangeAnimation(("DestroyL"));
		}
		break;
	default:
		break;
	}

}