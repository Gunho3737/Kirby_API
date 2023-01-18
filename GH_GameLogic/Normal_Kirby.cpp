#include "Normal_Kirby.h"
#include <GH_GameEngineWindow.h>
#include <GH_GameEngineTime.h>
#include <GH_GameEngineImage.h>
#include <GH_ResourcesManager.h>
#include <GH_GameEngineInput.h>
#include <GH_LevelManager.h>
#include <GH_Renderer.h>
#include <GH_Level.h>
#include "GameLogicEnum.h"
#include <GH_GameEngineCollision.h>
#include "One_One_Back.h"
#include "WeakMonster.h"
#include "Bullet.h"
#include "BeamMonster.h"
#include "CutterMonster.h"
#include "TreeBoss.h"
#include <GH_GameEngineSoundPlayer.h>
#include "Stage_one_one.h"

int Normal_Kirby::BackGroundSizeforCamX = 0;
float Normal_Kirby::DamageTime = 0.5f;
bool Normal_Kirby::DrainMonsterActive = false;
bool Normal_Kirby::ShootMoveActive = false;

Normal_Kirby::Normal_Kirby()
	: Speed(400.0f), JumpDir_({ 0,0 }), FlyDir_({ 0,0 }), DebugState_(DEBUGMODE::PLAYMODE), PlayerPivot_({0, -30}), HP(6)
{
	
	//GH_SetPos(GH_GameEngineWindow::GH_GetInst().GH_GetSize().GH_halffloat4());

	//GH_SetPos({ 150,100 });
	GH_SetUpdateOrder(10);
	GH_SetRenderOrder(3);

}

Normal_Kirby::~Normal_Kirby()
{
}

Normal_Kirby::Normal_Kirby(Normal_Kirby&& _Other) noexcept
{
}

std::string Normal_Kirby::GH_GetDirString()
{
	//내가 오른쪽인 상태면 문자열 "R"을,
	//내가 왼쪽인 상태면 문자열 "L"을 리턴해주는 함수
	switch (dirstate_)
	{
	case GH_DirectionState::LEFT:
		return "L";
		break;
	case GH_DirectionState::RIGHT:
		return "R";
		break;
	default:
		GH_GameEngineDebug::Assert();
		break;
	}

	GH_GameEngineDebug::Assert();
	return "";
}

void Normal_Kirby::MakeBreath()
{
	Bullet* BulletPtr = Normal_Kirby::GH_Actor::GH_GetLevel()->GH_CreateActor<Bullet>();

	if (dirstate_ == GH_DirectionState::LEFT)
	{
		BulletPtr->BulletDir_ = BulletDirectionState::LEFT;
	}
	else
	{
		BulletPtr->BulletDir_ = BulletDirectionState::RIGHT;
	}
	BulletPtr->GH_ChangeState(GH_BulletState::KirbyBreath);
	BulletPtr->GH_SetPos(GH_GetPos());
}


void Normal_Kirby::GH_LeftRightCheck()
{
	if (ShootMoveActive == true || DrainMonsterActive == true)
	{
		//몬스터의 흡수가 시작됐거나 무언가를 발사하는 애니메이션이 시작됐으면 좌우입력 체크를 막는다
		return;
	}

	GH_DirectionState PrevDirectionState = dirstate_;
	//내 지금의 LEFT,RIGHT 상태를 받는 Prev

	//좌우체크를 어디서 하느냐에 따라 들어가는 위치가 다를것이다
	if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
	{
		dirstate_ = GH_DirectionState::LEFT;
	}
	else if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R"))
	{
		dirstate_ = GH_DirectionState::RIGHT;
	}


	//키입력이 들어와서 방향이 바뀌엇을땐 애니메이션을 이곳에서 바꿔준다
	if (PrevDirectionState != dirstate_)
	{
		
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
			if (stateType_ == GH_PlayerState::DrainING)
			{
				DrainEffectRender_->GH_ChangeAnimation("DrainAfterDrainEffect" + GH_GetDirString());
			}
			break;
		case GH_KirbyAbilityState::DrainAfter:
			DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
			break;
		case GH_KirbyAbilityState::Beam:
			BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
			break;
		case GH_KirbyAbilityState::Cutter:
			CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
			break;
		default:
			break;
		}
		
		//애니메이션 이름은 내 상태이름+ L or R 이므로
		//내가 왼쪽상태면 L붙은 애니메이션을,
		//오른쪽 상태면 R붙은 애니메이션을 렌더할 것이다
	}

}

void Normal_Kirby::MonsterCollisionCheck()
{
	GH_GameEngineCollision* Collision = BodyCheckCollision->GH_CollisionGroupCheckOne(GAMECOLLISIONGROUP::MONSTER);
	
	
	if (nullptr != Collision) //무언가 Monster안에 들어있는것들 중에 충돌이 있었을경우
	{
		std::string CheckMonster = Collision->GH_GetActor()->GH_GetName();
		//몬스터 클래스에서 NameBase의 SetName()를 해줘서 이름을 붙히고, 그 이름을 받아온다

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

		//데미지를 입으면, 무조건 노말상태로 돌아오게 된다
		GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DamageUp);
	}

	
}

void Normal_Kirby::DrainCollisionCheck()
{
	GH_GameEngineCollision* Collision = DrainAttackCollision->GH_CollisionGroupCheckOne(GAMECOLLISIONGROUP::MONSTER);
	

	if (nullptr != Collision)
	{
		

	
		GH_GameEngineCollision* DrainEndCollision = DrainCheckCollision->GH_CollisionGroupCheckOne(GAMECOLLISIONGROUP::MONSTER);
		std::string CheckMonster = Collision->GH_GetActor()->GH_GetName();

		if (CheckMonster == "TreeBoss")
		{
			return;
		}

		DrainMonsterActive = true;
		DrainCheckCollision->GH_SetSize({ 70, 70 });
		if (CheckMonster == "WeakMonster")
		{
			DrainedMonsterState_ = "WeakMonster";
			WeakMonster* WeakMonsterPtr = Collision->GH_GetActorConvert<WeakMonster>();
			
			WeakMonsterPtr->GH_ChangeState(GH_MonsterState::GetDrain);

			GH_float4 Distance = GH_GetPos() - WeakMonsterPtr->GH_GetPos();	

			if (GH_DirectionState::LEFT == dirstate_)
			{ 
				//내 위치에 맞춰서 끌려오게 만들어서 점프중/낙하중에도 흡수가 가능하도록 했다
				WeakMonsterPtr->GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					WeakMonsterPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 200.0f);
				}
				else if (0 > Distance.y)
				{
					WeakMonsterPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}
			else if (GH_DirectionState::RIGHT == dirstate_)
			{
				WeakMonsterPtr->GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					WeakMonsterPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 200.0f);
				}
				else if (0 > Distance.y)
				{
					WeakMonsterPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}

			if (nullptr != DrainEndCollision)
			{
				EffectPlayer->GH_Stop();
				EffectPlayer->GH_PlayCountReset(1);
				HoldingAbilityStarType_ = GH_KirbyAbilityState::Normal;
				DrainAttackCollision->GH_SetSize({ 0,0 });
				DrainCheckCollision->GH_SetSize({ 0, 0 });
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Stand);
				DrainMonsterActive = false;
				WeakMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
			}
		}


		if (CheckMonster == "BeamMonster")
		{
			DrainedMonsterState_ = "BeamMonster";
			BeamMonster* BeamMonsterPtr = Collision->GH_GetActorConvert<BeamMonster>();

			BeamMonsterPtr->GH_ChangeState(GH_MonsterState::GetDrain);

			GH_float4 Distance = GH_GetPos() - BeamMonsterPtr->GH_GetPos();

			if (GH_DirectionState::LEFT == dirstate_)
			{
				//내 위치에 맞춰서 끌려오게 만들어서 점프중/낙하중에도 흡수가 가능하도록 했다
				BeamMonsterPtr->GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					BeamMonsterPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
				else if (0 > Distance.y)
				{
					BeamMonsterPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}
			else if (GH_DirectionState::RIGHT == dirstate_)
			{
				BeamMonsterPtr->GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					BeamMonsterPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 200.0f);
				}
				else if (0 > Distance.y)
				{
					BeamMonsterPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}

			if (nullptr != DrainEndCollision)
			{
				EffectPlayer->GH_Stop();
				EffectPlayer->GH_PlayCountReset(1);
				HoldingAbilityStarType_ = GH_KirbyAbilityState::Beam;
				DrainAttackCollision->GH_SetSize({ 0,0 });
				DrainCheckCollision->GH_SetSize({ 0, 0 });
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Stand);
				DrainMonsterActive = false;
				BeamMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
			}
		}


		if (CheckMonster == "CutterMonster")
		{
			DrainedMonsterState_ = "CutterMonster";
			CutterMonster* CutterMonsterPtr = Collision->GH_GetActorConvert<CutterMonster>();

			CutterMonsterPtr->GH_ChangeState(GH_MonsterState::GetDrain);

			GH_float4 Distance = GH_GetPos() - CutterMonsterPtr->GH_GetPos();

			if (GH_DirectionState::LEFT == dirstate_)
			{
				//내 위치에 맞춰서 끌려오게 만들어서 점프중/낙하중에도 흡수가 가능하도록 했다
				CutterMonsterPtr->GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					CutterMonsterPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 200.0f);
				}
				else if (0 > Distance.y)
				{
					CutterMonsterPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}
			else if (GH_DirectionState::RIGHT == dirstate_)
			{
				CutterMonsterPtr->GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					CutterMonsterPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 200.0f);
				}
				else if (0 > Distance.y)
				{
					CutterMonsterPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}

			if (nullptr != DrainEndCollision)
			{
				EffectPlayer->GH_Stop();
				EffectPlayer->GH_PlayCountReset(1);
				HoldingAbilityStarType_ = GH_KirbyAbilityState::Cutter;
				DrainAttackCollision->GH_SetSize({ 0,0 });
				DrainCheckCollision->GH_SetSize({ 0,0 });
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Stand);
				DrainMonsterActive = false;
				CutterMonsterPtr->GH_ChangeState(GH_MonsterState::Dead);
			}
		}


		if (CheckMonster == "Apple")
		{
			DrainedMonsterState_ = "WeakMonster";
			Bullet* BulletPtr = Collision->GH_GetActorConvert<Bullet>();

			BulletPtr->GH_ChangeState(GH_BulletState::GetDrain);

			GH_float4 Distance = GH_GetPos() - BulletPtr->GH_GetPos();

			if (GH_DirectionState::LEFT == dirstate_)
			{
				//내 위치에 맞춰서 끌려오게 만들어서 점프중/낙하중에도 흡수가 가능하도록 했다
				BulletPtr->GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					BulletPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 200.0f);
				}
				else if (0 > Distance.y)
				{
					BulletPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}
			else if (GH_DirectionState::RIGHT == dirstate_)
			{
				BulletPtr->GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				if (0 < Distance.y)
				{
					BulletPtr->GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 200.0f);
				}
				else if (0 > Distance.y)
				{
					BulletPtr->GH_SetMove(GH_float4::UP * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
				}
			}

			if (nullptr != DrainEndCollision)
			{
				EffectPlayer->GH_Stop();
				EffectPlayer->GH_PlayCountReset(1);
				HoldingAbilityStarType_ = GH_KirbyAbilityState::Normal;
				DrainAttackCollision->GH_SetSize({ 0,0 });
				DrainCheckCollision->GH_SetSize({ 0,0 });
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Stand);
				DrainMonsterActive = false;
				BulletPtr->GH_ChangeState(GH_BulletState::Dead);
			}
		}

	}
}

void Normal_Kirby::MonsterAttackCollisionCheck()
{
	GH_GameEngineCollision* Collision = BodyCheckCollision->GH_CollisionGroupCheckOne(GAMECOLLISIONGROUP::MONSTERATTACK);


	if (nullptr != Collision) //무언가 Monster안에 들어있는것들 중에 충돌이 있었을경우
	{
		//데미지를 입으면, 무조건 노말상태로 돌아오게 된다

		GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DamageUp);
	}


}


void Normal_Kirby::BeamCollisionCheck()
{
	GH_GameEngineCollision* Collision = BeamAttackCollision->GH_CollisionGroupCheckOne(GAMECOLLISIONGROUP::MONSTER);


	if (nullptr != Collision) //무언가 Monster안에 들어있는것들 중에 충돌이 있었을경우
	{
		std::string CheckMonster = Collision->GH_GetActor()->GH_GetName();
		//몬스터 클래스에서 NameBase의 SetName()를 해줘서 이름을 붙히고, 그 이름을 받아온다

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
	}

}

void Normal_Kirby::DoorCollisionCheck()
{
	if (true == BodyCheckCollision->GH_CollisionCheck(One_One_Back::DoorCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("U"))
		{
			EffectPlayer->GH_Stop();
			EffectPlayer->GH_PlayAlone("KirbyDoorIn.wav");
			GH_GetLevel()->ChangeStage();
		}
	}
	
}

void Normal_Kirby::GH_Start()
{
	GH_GameEngineImage* FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("1-1_big.bmp");
	BackGroundSizeforCamX = FindImage->GH_GetSize().ix();
	//카메라 이동 제한을 위해서 배경의 크기를 받아옴

	if (false == GH_GameEngineInput::GH_GetInst().GH_IsKey("L")) 
	{
		//VK_ 시리즈=> 키보드 방향키입력
		//키를 설정 할때는 대문자만 넣을 수 있다
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("L", VK_LEFT);
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("R", VK_RIGHT);
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("U", VK_UP);
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("D", VK_DOWN);
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("Att", 'X');
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("Jump", 'Z');
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("ChangeDebugMode", 'R');
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("BackToNormal", 'F');
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("BeamChange", 'S');
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("CutterChange", 'D');
	}

	//기본 커비 렌더 코드
	{
		int SpriteReverse = 160;
		
		MainRender_ = GH_CreateRenderer("normal_kirby.bmp");
		MainRender_->GH_CreateAnimation("NormalStandR", "normal_kirby.bmp", 13, 15, true, 0.3f);
		MainRender_->GH_CreateAnimation("NormalStandL", "normal_kirby.bmp", SpriteReverse + 13, SpriteReverse + 15, true, 0.3f);
		MainRender_->GH_CreateAnimation("NormalDownR", "normal_kirby.bmp", 88, 88, false);
		MainRender_->GH_CreateAnimation("NormalDownL", "normal_kirby.bmp", SpriteReverse + 88, SpriteReverse + 88, false);
		MainRender_->GH_CreateAnimation("NormalDrainReadyR", "normal_kirby.bmp", 110, 111, false, 0.07f);
		MainRender_->GH_CreateAnimation("NormalDrainReadyL", "normal_kirby.bmp", SpriteReverse + 110, SpriteReverse + 111, true, 0.07f);
		MainRender_->GH_CreateAnimation("NormalDrainINGR", "normal_kirby.bmp", 112, 113, true, 0.07f);
		MainRender_->GH_CreateAnimation("NormalDrainINGL", "normal_kirby.bmp", SpriteReverse + 112, SpriteReverse + 113, true, 0.07f);
		MainRender_->GH_CreateAnimation("NormalWalkR", "normal_kirby.bmp", 52, 61);
		MainRender_->GH_CreateAnimation("NormalWalkL", "normal_kirby.bmp", SpriteReverse + 52, SpriteReverse + 61);
		MainRender_->GH_CreateAnimation("NormalJumpUpR", "normal_kirby.bmp", 81, 81, false);
		MainRender_->GH_CreateAnimation("NormalJumpUpL", "normal_kirby.bmp", SpriteReverse + 81, SpriteReverse + 81, false);
		MainRender_->GH_CreateAnimation("NormalJumpDownR", "normal_kirby.bmp", 82, 87, false, 0.05f);
		MainRender_->GH_CreateAnimation("NormalJumpDownL", "normal_kirby.bmp", SpriteReverse + 82, SpriteReverse + 87, false, 0.05f);
		MainRender_->GH_CreateAnimation("NormalFlyR", "normal_kirby.bmp", 143, 153, false, 0.05f);
		MainRender_->GH_CreateAnimation("NormalFlyL", "normal_kirby.bmp", SpriteReverse+ 143, SpriteReverse +153, false, 0.05f);
		MainRender_->GH_CreateAnimation("NormalFlyEndR", "normal_kirby.bmp", 154,156, false, 0.07f);
		MainRender_->GH_CreateAnimation("NormalFlyEndL", "normal_kirby.bmp", SpriteReverse + 154, SpriteReverse + 156, false, 0.07f);
		MainRender_->GH_CreateAnimation("NormalDamageUpR", "normal_kirby.bmp",  137, 137, false);
		MainRender_->GH_CreateAnimation("NormalDamageUpL", "normal_kirby.bmp", SpriteReverse + 137, SpriteReverse + 137, false);
		MainRender_->GH_CreateAnimation("NormalDamageDownR", "normal_kirby.bmp",  138, 142, false, 0.05f);
		MainRender_->GH_CreateAnimation("NormalDamageDownL", "normal_kirby.bmp", SpriteReverse + 138, SpriteReverse + 142, false, 0.05f);
		MainRender_->GH_CreateAnimation("NormalDead", "normal_kirby.bmp", SpriteReverse * 2 + 0, SpriteReverse * 2 + 0, false, 0.05f);
		MainRender_->GH_CreateAnimation("NormalSpin", "normal_kirby.bmp", SpriteReverse*2 + 0, SpriteReverse*2 + 14, true, 0.02f);
		//StateName을 사용하므로, 애니메이션 이름은 Enum GH_PlayerState와 일치시키는게 좋음
		//왼쪽을 바라보는 애니메이션의 경우 맨뒤에 L을, 오른쪽을 바라보는 경우 R을 붙힌다
	}

	//흡수한 커비 렌더 코드
	{
		int SpriteReverse = 160;
		
		DrainAfterRender_ = GH_CreateRenderer("normal_kirby.bmp");
		DrainAfterRender_->GH_CreateAnimation("DrainAfterStandR", "normal_kirby.bmp", 43, 45, true, 0.3f);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterStandL", "normal_kirby.bmp", SpriteReverse + 43, SpriteReverse + 45, true, 0.3f);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterDownR", "normal_kirby.bmp", 97, 97, false);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterDownL", "normal_kirby.bmp", SpriteReverse + 97, SpriteReverse + 97, false);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterWalkR", "normal_kirby.bmp", 71, 80, true);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterWalkL", "normal_kirby.bmp", SpriteReverse + 71, SpriteReverse + 80, true);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterJumpUpR", "normal_kirby.bmp", 92, 93, false, 0.15f);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterJumpUpL", "normal_kirby.bmp", SpriteReverse + 92, SpriteReverse + 93, false, 0.15f);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterJumpDownR", "normal_kirby.bmp", 94, 96, false);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterJumpDownL", "normal_kirby.bmp", SpriteReverse + 94, SpriteReverse + 96, false);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterAttackR", "normal_kirby.bmp", 154, 156, false, 0.05f);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterAttackL", "normal_kirby.bmp", SpriteReverse + 154, SpriteReverse + 156, false, 0.05f);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterEatR", "normal_kirby.bmp", 123, 126, false, 0.05f);
		DrainAfterRender_->GH_CreateAnimation("DrainAfterEatL", "normal_kirby.bmp", SpriteReverse+123, SpriteReverse + 126, false, 0.05f);
	}

	//커터 커비 렌더 코드
	{
		int SpriteReverse = 50;
		CutterRender_ = GH_CreateRenderer("cutter_kirby.bmp");
		CutterRender_->GH_CreateAnimation("CutterStandR", "cutter_kirby.bmp", 0, 2, true, 0.3f);
		CutterRender_->GH_CreateAnimation("CutterStandL", "cutter_kirby.bmp", SpriteReverse + 0, SpriteReverse + 2, true, 0.3f);
		CutterRender_->GH_CreateAnimation("CutterDownR", "cutter_kirby.bmp", 30, 30, false);
		CutterRender_->GH_CreateAnimation("CutterDownL", "cutter_kirby.bmp", SpriteReverse + 30, SpriteReverse + 30);
		CutterRender_->GH_CreateAnimation("CutterWalkR", "cutter_kirby.bmp", 3, 12, true);
		CutterRender_->GH_CreateAnimation("CutterWalkL", "cutter_kirby.bmp", SpriteReverse + 3, SpriteReverse + 12, true);
		CutterRender_->GH_CreateAnimation("CutterJumpUpR", "cutter_kirby.bmp", 22, 22, false);
		CutterRender_->GH_CreateAnimation("CutterJumpUpL", "cutter_kirby.bmp", SpriteReverse + 22, SpriteReverse + 22, false);
		CutterRender_->GH_CreateAnimation("CutterJumpDownR", "cutter_kirby.bmp", 23, 28, false, 0.05f);
		CutterRender_->GH_CreateAnimation("CutterJumpDownL", "cutter_kirby.bmp", SpriteReverse + 23, SpriteReverse + 28, false, 0.05f);
		CutterRender_->GH_CreateAnimation("CutterFlyR", "cutter_kirby.bmp", 31, 41, false, 0.05f);
		CutterRender_->GH_CreateAnimation("CutterFlyL", "cutter_kirby.bmp", SpriteReverse + 31, SpriteReverse + 41, false, 0.05f);
		CutterRender_->GH_CreateAnimation("CutterFlyEndR", "cutter_kirby.bmp", 42, 44, false, 0.07f);
		CutterRender_->GH_CreateAnimation("CutterFlyEndL", "cutter_kirby.bmp", SpriteReverse + 42, SpriteReverse + 44, true, 0.07f);
		CutterRender_->GH_CreateAnimation("CutterAttackR", "cutter_kirby.bmp", 45, 49, false, 0.04f);
		CutterRender_->GH_CreateAnimation("CutterAttackL", "cutter_kirby.bmp", SpriteReverse + 45, SpriteReverse + 49, false, 0.04f);
	}

	//빔커비 렌더 코드
	{
		int SpriteReverse = 60;
		BeamRender_ = GH_CreateRenderer("beam_kirby.bmp");
		BeamRender_->GH_CreateAnimation("BeamStandR", "beam_kirby.bmp", 0, 2, true, 0.3f);
		BeamRender_->GH_CreateAnimation("BeamStandL", "beam_kirby.bmp", SpriteReverse + 0, SpriteReverse + 2, true, 0.3f);
		BeamRender_->GH_CreateAnimation("BeamDownR", "beam_kirby.bmp", 33, 33, false);
		BeamRender_->GH_CreateAnimation("BeamDownL", "beam_kirby.bmp", SpriteReverse + 33, SpriteReverse + 33, false);
		BeamRender_->GH_CreateAnimation("BeamWalkR", "beam_kirby.bmp", 3, 14, true);
		BeamRender_->GH_CreateAnimation("BeamWalkL", "beam_kirby.bmp", SpriteReverse + 3, SpriteReverse + 14);
		BeamRender_->GH_CreateAnimation("BeamJumpUpR", "beam_kirby.bmp", 25, 25, false);
		BeamRender_->GH_CreateAnimation("BeamJumpUpL", "beam_kirby.bmp", SpriteReverse + 25, SpriteReverse + 25, false);
		BeamRender_->GH_CreateAnimation("BeamJumpDownR", "beam_kirby.bmp", 26, 31, false, 0.05f);
		BeamRender_->GH_CreateAnimation("BeamJumpDownL", "beam_kirby.bmp", SpriteReverse + 26, SpriteReverse + 31, false, 0.05f);
		BeamRender_->GH_CreateAnimation("BeamFlyR", "beam_kirby.bmp", 37, 47, false, 0.05f);
		BeamRender_->GH_CreateAnimation("BeamFlyL", "beam_kirby.bmp", SpriteReverse + 37, SpriteReverse + 47, false, 0.05f);
		BeamRender_->GH_CreateAnimation("BeamFlyEndR", "beam_kirby.bmp", 34, 36, false, 0.07f);
		BeamRender_->GH_CreateAnimation("BeamFlyEndL", "beam_kirby.bmp", SpriteReverse + 34, SpriteReverse + 36, false, 0.07f);
		BeamRender_->GH_CreateAnimation("BeamAttackR", "beam_kirby.bmp", 48, 57, false, 0.05f);
		BeamRender_->GH_CreateAnimation("BeamAttackL", "beam_kirby.bmp", SpriteReverse + 48, SpriteReverse + 57, false, 0.05f);
	}

	//흡수공격 이펙트 렌더 코드
	{
		DrainEffectRender_ = GH_CreateRenderer("Kirby_Drain.bmp");
		DrainEffectRender_->GH_CreateAnimation("DrainAfterDrainEffectR", "Kirby_Drain.bmp", 0, 4, true, 0.02f);
		DrainEffectRender_->GH_CreateAnimation("DrainAfterDrainEffectL", "Kirby_Drain.bmp", 5, 9, true, 0.02f);
		DrainEffectRender_->GH_SetPivotPos(PlayerPivot_ + GH_float4::RIGHT * 100 );
	}

	//빔 공격 이펙트 렌더 코드
	{
		BeamEffectRender_ = GH_CreateRenderer("Beam_Att.bmp");
		BeamEffectRender_->GH_CreateAnimation("BeamAttackEffectR", "Beam_Att.bmp", 0, 9, false, 0.05f);
		BeamEffectRender_->GH_CreateAnimation("BeamAttackEffectL", "Beam_Att.bmp", 10, 19, false, 0.05f);
		BeamEffectRender_->GH_SetPivotPos(PlayerPivot_ + GH_float4::RIGHT * 100);
	}
	
	//커비가 삼킬때의 이펙트 렌더 코드
	{
		EatEffectRender_ = GH_CreateRenderer("Eat_Effect.bmp");
		EatEffectRender_->GH_CreateAnimation("EatEffect", "Eat_Effect.bmp", 0, 5, false, 0.04f);
	}

	//커비 댄스용 렌더 코드
	{
		DanceRender_ = GH_CreateRenderer("KirbyDance_one.bmp");
		DanceRender_->GH_CreateAnimation("KirbyDanceWait", "KirbyDance_one.bmp", 1, 1, false);
		DanceRender_->GH_CreateAnimation("KirbyDance", "KirbyDance_one.bmp", 1, 19, false, 0.085f);

		DanceRendertwo_ = GH_CreateRenderer("KirbyDance_two.bmp");
		DanceRendertwo_->GH_CreateAnimation("KirbyDanceWaittwo", "KirbyDance_two.bmp", 0, 0, false);
		DanceRendertwo_->GH_CreateAnimation("KirbyDancetwo", "KirbyDance_two.bmp", 0, 19, false, 0.085f);

		DanceRendertwo_->GH_ChangeAnimation("KirbyDanceWaittwo");
		DanceRendertwo_->GH_Off();
	
		DanceRenderthree_ = GH_CreateRenderer("KirbyDance_three.bmp");
		DanceRenderthree_->GH_CreateAnimation("KirbyDanceWaitthree", "KirbyDance_three.bmp", 0, 0, false);
		DanceRenderthree_->GH_CreateAnimation("KirbyDancethree", "KirbyDance_three.bmp", 0, 19, false, 0.085f);

		DanceRenderthree_->GH_ChangeAnimation("KirbyDanceWaitthree");
		DanceRenderthree_->GH_Off();

	}
	dirstate_ = GH_DirectionState::RIGHT;

	GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
	HoldingAbilityStarType_ = GH_KirbyAbilityState::Normal;

	MiddleGroundCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::POINT);
	RightGroundCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::POINT);
	RightGroundCheckCollision->GH_SetPivot({ 30, -30 });
	LeftGroundCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::POINT);
	LeftGroundCheckCollision->GH_SetPivot({ -35,-30 });
	TopGroundCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::POINT);
	TopGroundCheckCollision->GH_SetPivot({ 0, -65});

	{ 
		//충돌을 일으킬 색을 설정
		//이 점이 가진 색정보와, 점과 충돌할 이미지의 좌표의 색이 같다면 PointToImage가 true를 리턴하게 되는 것이다
		MiddleGroundCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		RightGroundCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		LeftGroundCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
		TopGroundCheckCollision->GH_SetColorCheck(RGB(0, 0, 0));
	}

	//커비 몸의 충돌판정
	BodyCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::RECTANGLE);
	BodyCheckCollision->GH_SetPivot(PlayerPivot_);
	BodyCheckCollision->GH_SetSize({ 75,70 });

	//흡수 공격의 충돌판정
	DrainAttackCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::RECTANGLE);
	DrainAttackCollision->GH_SetSize({ 0,0 });
	DrainAttackCollision->GH_SetPivot(PlayerPivot_ + GH_float4::RIGHT*100);

	//끌려들어오는 적들을 받기 위한 충돌판정
	DrainCheckCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::RECTANGLE);
	DrainCheckCollision->GH_SetSize({ 0,0 });
	DrainCheckCollision->GH_SetPivot(PlayerPivot_ + GH_float4::RIGHT*50);
	
	//빔 공격을 위한 충돌판정
	BeamAttackCollision = GH_CreateCollision(GAMECOLLISIONGROUP::PLAYER, GH_CollisionCheckType::RECTANGLE);
	BeamAttackCollision->GH_SetSize({0,0});
	BeamAttackCollision->GH_SetPivot(PlayerPivot_ + GH_float4::RIGHT*100);


	EffectPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	if (EffectPlayer == nullptr)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 플레이어 생성이 되지 않았습니다");
	}

	//재생 횟수를 매번 설정 해 주는 PlayCoundReset
	EffectPlayer->GH_PlayCountReset(1);

}

void Normal_Kirby::GH_ChangeState(GH_KirbyAbilityState _AbilityType, GH_PlayerState _StateType)
{
	AbilityType_ = _AbilityType;
	stateType_ = _StateType;


	//나의 상태가 들어오면 애니메이션을 재생시킨다
	//string으로 내 능력상태와 행동상태를 같은 이름으로 만든다음,
	//함수안에서 string+를 이용해서 내가 만들어둔 애니메이션 이름과 같은 이름으로 만든다.
	//그리고 그 애니메이션을 그 string을 받아서 사용
	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
	{
		switch (stateType_)
		{
		case GH_PlayerState::Stand:
			AbilityStateName_ = "Normal";
			StateName_ = "Stand";
			GH_StartStand();
			break;
		case GH_PlayerState::Down:
			AbilityStateName_ = "Normal";
			StateName_ = "Down";
			GH_StartDown();
			break;
		case GH_PlayerState::Walk:
			AbilityStateName_ = "Normal";
			StateName_ = "Walk";
			GH_StartWalk();
			break;
		case GH_PlayerState::DrainReady:
			AbilityStateName_ = "Normal";
			StateName_ = "DrainReady";
			GH_StartDrainReady();
			break;
		case GH_PlayerState::DrainING:
			AbilityStateName_ = "Normal";
			StateName_ = "DrainING";
			GH_StartDrainING();
			break;
		case GH_PlayerState::JumpUp:
			AbilityStateName_ = "Normal";
			StateName_ = "JumpUp";
			JumpDir_ = GH_float4::UP * 700.0f;
			GH_StartJumpUp();
			break;
		case GH_PlayerState::JumpDown:
			AbilityStateName_ = "Normal";
			StateName_ = "JumpDown";
			GH_StartJumpDown();
			break;
		case GH_PlayerState::DamageUp:
			AbilityStateName_ = "Normal";
			DamageTime = 0.5f;
			StateName_ = "DamageUp";
			GH_StartDamageUp();
			break;
		case GH_PlayerState::DamageDown:
			AbilityStateName_ = "Normal";
			StateName_ = "DamageDown";
			GH_StartDamageDown();
			break;
		case GH_PlayerState::Fly:
			FlyDir_ = GH_float4::UP * 400.0f;
			AbilityStateName_ = "Normal";
			StateName_ = "Fly";
			GH_StartFly();
			break;
		case GH_PlayerState::FlyEnd:
			AbilityStateName_ = "Normal";
			StateName_ = "FlyEnd";
			GH_StartFlyEnd();
			break;
		case GH_PlayerState::Dead:
			DamageTime = 1.0f;
			JumpDir_ = GH_float4::UP * 1000.0f;
			GH_StartDead();
			break;
		case GH_PlayerState::DanceReady:
			GH_StartDanceReady();
			break;
		case GH_PlayerState::Dance:
			GH_StartDance();
			break;
		default:
			break;
		}
	}
		break;
	case GH_KirbyAbilityState::DrainAfter:
		switch (stateType_)
		{
		case GH_PlayerState::Stand:
			AbilityStateName_ = "DrainAfter";
			StateName_ = "Stand";
			GH_StartStand();
			break;
		case GH_PlayerState::Down:
			AbilityStateName_ = "DrainAfter";
			StateName_ = "Down";
			GH_StartDown();
			break;
		case GH_PlayerState::Walk:
			AbilityStateName_ = "DrainAfter";
			StateName_ = "Walk";
			GH_StartWalk();
			break;
		case GH_PlayerState::JumpUp:
			AbilityStateName_ = "DrainAfter";
			StateName_ = "JumpUp";
			JumpDir_ = GH_float4::UP * 700.0f;
			GH_StartJumpUp();
			break;
		case GH_PlayerState::JumpDown:
			AbilityStateName_ = "DrainAfter";
			StateName_ = "JumpDown";
			GH_StartJumpDown();
			break;
		case GH_PlayerState::Attack:
			AbilityStateName_ = "DrainAfter";
			StateName_ = "Attack";
			GH_StartAttack();
			break;
		case GH_PlayerState::Eat:
			AbilityStateName_ = "DrainAfter";
			StateName_ = "Eat";
			GH_StartEat();
			break;
		default:
			break;
		}
		break;
	case GH_KirbyAbilityState::Beam:
		switch (stateType_)
		{
		case GH_PlayerState::Stand:
			AbilityStateName_ = "Beam";
			StateName_ = "Stand";
			GH_StartStand();
			break;
		case GH_PlayerState::Down:
			AbilityStateName_ = "Beam";
			StateName_ = "Down";
			GH_StartDown();
			break;
		case GH_PlayerState::Walk:
			AbilityStateName_ = "Beam";
			StateName_ = "Walk";
			GH_StartWalk();
			break;
		case GH_PlayerState::JumpUp:
			AbilityStateName_ = "Beam";
			StateName_ = "JumpUp";
			JumpDir_ = GH_float4::UP * 700.0f;
			GH_StartJumpUp();
			break;
		case GH_PlayerState::JumpDown:
			AbilityStateName_ = "Beam";
			StateName_ = "JumpDown";
			GH_StartJumpDown();
			break;
		case GH_PlayerState::Fly:
			FlyDir_ = GH_float4::UP * 400.0f;
			AbilityStateName_ = "Beam";
			StateName_ = "Fly";
			GH_StartFly();
			break;
		case GH_PlayerState::FlyEnd:
			AbilityStateName_ = "Beam";
			StateName_ = "FlyEnd";
			GH_StartFlyEnd();
			break;
		case GH_PlayerState::Attack:
			AbilityStateName_ = "Beam";
			StateName_ = "Attack";
			GH_StartAttack();
			break;
		default:
			break;
		}
		break;
	case GH_KirbyAbilityState::Cutter:
		switch (stateType_)
		{
		case GH_PlayerState::Stand:
			AbilityStateName_ = "Cutter";
			StateName_ = "Stand";
			GH_StartStand();
			break;
		case GH_PlayerState::Down:
			AbilityStateName_ = "Cutter";
			StateName_ = "Down";
			GH_StartDown();
			break;
		case GH_PlayerState::Walk:
			AbilityStateName_ = "Cutter";
			StateName_ = "Walk";
			GH_StartWalk();
			break;
		case GH_PlayerState::JumpUp:
			AbilityStateName_ = "Cutter";
			StateName_ = "JumpUp";
			JumpDir_ = GH_float4::UP * 700.0f;
			GH_StartJumpUp();
			break;
		case GH_PlayerState::JumpDown:
			AbilityStateName_ = "Cutter";
			StateName_ = "JumpDown";
			GH_StartJumpDown();
			break;
		case GH_PlayerState::Fly:
			FlyDir_ = GH_float4::UP * 400.0f;
			AbilityStateName_ = "Cutter";
			StateName_ = "Fly";
			GH_StartFly();
			break;
		case GH_PlayerState::FlyEnd:
			AbilityStateName_ = "Cutter";
			StateName_ = "FlyEnd";
			GH_StartFlyEnd();
			break;
		case GH_PlayerState::Attack:
			AbilityStateName_ = "Cutter";
			StateName_ = "Attack";
			GH_StartAttack();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	
	

}

void Normal_Kirby::GH_StartStand()
{
	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::DrainAfter:
		DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
	
	//애니메이션 이름은 내 상태이름+ L or R 이므로
	//내가 왼쪽상태면 L붙은 애니메이션을,
	//오른쪽 상태면 R붙은 애니메이션을 렌더할 것이다

}

void Normal_Kirby::GH_StartDown()
{
	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::DrainAfter:
		DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
}

void Normal_Kirby::GH_StartWalk()
{
	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::DrainAfter:
		DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
}



void Normal_Kirby::GH_StartDrainReady()
{

	MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
}

void Normal_Kirby::GH_StartDrainING()
{
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);
	MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
	DrainEffectRender_->GH_ChangeAnimation("DrainAfterDrainEffect" + GH_GetDirString());

}

void Normal_Kirby::GH_StartJumpUp()
{
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);

	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::DrainAfter:
		DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
}

void Normal_Kirby::GH_StartJumpDown()
{
	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::DrainAfter:
		DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
}

void Normal_Kirby::GH_StartDamageUp()
{
	HP -= 1;
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);

	//데미지를 입으면 무조건 노말상태로 돌아옴
	MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());

	EatEffectRender_->GH_ChangeAnimation("EatEffect", true);
}

void Normal_Kirby::GH_StartDamageDown()
{
	
	MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
}

void Normal_Kirby::GH_StartFly()
{
	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
	
}

void Normal_Kirby::GH_StartFlyEnd()
{
	MakeBreath();
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);

	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
	
}

void Normal_Kirby::GH_StartAttack()
{
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);

	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::DrainAfter:
		DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		BeamEffectRender_->GH_ChangeAnimation("BeamAttackEffect" + GH_GetDirString(), true);
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());
		break;
	default:
		break;
	}
}

void Normal_Kirby::GH_StartEat()
{
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);

	DrainAfterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString());

	if (HoldingAbilityStarType_ != GH_KirbyAbilityState::Normal)
	{
		EatEffectRender_->GH_ChangeAnimation("EatEffect", true);
	
	}
	
}

void Normal_Kirby::GH_StartDead()
{
	
	EffectPlayer->GH_PlayCountReset(1);


	MainRender_->GH_ChangeAnimation("NormalDead");



}

void Normal_Kirby::GH_StartDanceReady()
{
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);
}

void Normal_Kirby::GH_StartDance()
{
	DamageTime = 2.0f;
	EffectPlayer->GH_Stop();
	EffectPlayer->GH_PlayCountReset(1);
	DanceRender_->GH_ChangeAnimation("KirbyDanceWait");
}

void Normal_Kirby::GH_Update()
{
	if (DebugState_ == DEBUGMODE::DEBUGMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		DebugState_ = DEBUGMODE::PLAYMODE;
	}
	else if (DebugState_ == DEBUGMODE::PLAYMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		DebugState_ = DEBUGMODE::DEBUGMODE;
	}


	switch (dirstate_)
	{
	case GH_DirectionState::LEFT:
	{
		DrainAttackCollision->GH_SetPivot(PlayerPivot_ + GH_float4::LEFT * 100);
		DrainCheckCollision->GH_SetPivot(PlayerPivot_ + GH_float4::LEFT * 30);
		BeamAttackCollision->GH_SetPivot(PlayerPivot_ + GH_float4::LEFT * 100);
		DrainEffectRender_->GH_SetPivotPos(PlayerPivot_ + GH_float4::LEFT * 100 + GH_float4::DOWN * 60); 
		//흡수 이미지의 위치는 좌우 100 + 아래로 60
		BeamEffectRender_->GH_SetPivotPos(PlayerPivot_);
	}
		break;
	case GH_DirectionState::RIGHT:
	{
		DrainAttackCollision->GH_SetPivot(PlayerPivot_ + GH_float4::RIGHT * 100);
		DrainCheckCollision->GH_SetPivot(PlayerPivot_ + GH_float4::RIGHT * 30);
		BeamAttackCollision->GH_SetPivot(PlayerPivot_ + GH_float4::RIGHT * 100);
		DrainEffectRender_->GH_SetPivotPos(PlayerPivot_ + GH_float4::RIGHT * 100 + GH_float4::DOWN * 60);
		BeamEffectRender_->GH_SetPivotPos(PlayerPivot_ + GH_float4::RIGHT * 120);
	}
		break;
	default:
		break;
	}


	switch (stateType_)
	{
	case GH_PlayerState::Stand:
		GH_Stand();
		break;
	case GH_PlayerState::Down:
		GH_Down();
		break;
	case GH_PlayerState::Walk:
		GH_Walk();
		break;
	case GH_PlayerState::DrainReady:
		GH_DrainReady();
		break;
	case GH_PlayerState::DrainING:
		GH_DrainING();
		break;
	case GH_PlayerState::JumpUp:
		GH_JumpUp();
		break;
	case GH_PlayerState::JumpDown:
		GH_JumpDown();
		break;
	case GH_PlayerState::DamageUp:
		GH_DamageUp();
		break;
	case GH_PlayerState::DamageDown:
		GH_DamageDown();
		break;
	case GH_PlayerState::Fly:
		GH_Fly();
		break;
	case GH_PlayerState::FlyEnd:
		GH_FlyEnd();
		break;
	case GH_PlayerState::Attack:
		GH_Attack();
		break;
	case GH_PlayerState::Eat:
		GH_Eat();
		break;
	case GH_PlayerState::Dead:
		GH_Dead();
		break;
	case GH_PlayerState::DanceReady:
		GH_DanceReady();
		break;
	case GH_PlayerState::Dance:
		GH_Dance();
		break;
	default:
		break;
	}



	//내 객체의 위치를 받아오는것으로 카메라의 위치를 정함
	GH_GetLevel()->GH_SetCamPos({(GH_GetPos() - GH_GameEngineWindow::GH_GetInst().GH_GetSize().GH_halffloat4()).x, 0 });

	//상하이동을 해도 카메라의 y축은 안바뀌게 제한
	if (0 > GH_GetLevel()->GH_GetCamPos().x)
	{
		//카메라가 음수 좌표이면, 0,0에 카메라를 고정
		GH_GetLevel()->GH_SetCamPos({ 0,0 });
	}

	if ((BackGroundSizeforCamX - GH_GameEngineWindow::GH_GetInst().GH_GetSize().x) < GH_GetLevel()->GH_GetCamPos().x)
	{
		//카메라 좌표가 배경 이미지크기 - 화면크기를 넘어간다면 카메라를 고정
		//배경 이미지 넓이 -> 3172

		GH_GetLevel()->GH_SetCamPos(
			{
				(BackGroundSizeforCamX - GH_GameEngineWindow::GH_GetInst().GH_GetSize().x), 0
			}
		);
	}


}

void Normal_Kirby::GH_Stand()
{
	MonsterCollisionCheck();
	GH_LeftRightCheck();
	MonsterAttackCollisionCheck();
	DoorCollisionCheck();
		
	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("BeamChange"))
	{
		GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Stand);
	}

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("CutterChange"))
	{
		GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Stand);
	}

	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		//뒷배경과의 충돌이 false라면
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);
			break;
		case GH_KirbyAbilityState::DrainAfter:
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Down);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Down);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Down);
			break;
		default:
			break;
		}	
	}


	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
	{

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")
			|| true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Walk);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DrainReady);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::JumpUp);
		}
	}
		break;
	case GH_KirbyAbilityState::DrainAfter:
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("BackToNormal"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
		}


		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")|| true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Walk);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Attack);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::JumpUp);
		}

		if (true == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision)) //땅바닥에 닿아있을때
		{
			if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("D"))
			{
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Eat);
			}
		}
	}
		break;
	case GH_KirbyAbilityState::Beam:
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("BackToNormal"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
		}


		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")
			|| true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Walk);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Attack);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::JumpUp);
		}
	}
	break;
		break;
	case GH_KirbyAbilityState::Cutter:
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("BackToNormal"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
		}


		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")
			|| true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Walk);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Attack);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::JumpUp);
		}
	}
		break;
	default:
		break;
	}
	
}

void Normal_Kirby::GH_Down()
{
	GH_LeftRightCheck();
	MonsterCollisionCheck();
	MonsterAttackCollisionCheck();

	if (false == RightGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R"))
		{
			GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	if (false == LeftGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}
	

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DrainReady);
			break;
		case GH_KirbyAbilityState::DrainAfter:
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Attack);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Attack);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Attack);
			break;
		default:
			break;
		}

	}

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Jump"))
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Fly);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Fly);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Fly);
			break;
		default:
			break;
		}
	}


	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
	}
	else
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
			break;
		case GH_KirbyAbilityState::DrainAfter:
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Stand);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Stand);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Stand);
			break;
		default:
			break;
		}
	}
}


void Normal_Kirby::GH_Walk()
{
	GH_LeftRightCheck();
	MonsterCollisionCheck();
	MonsterAttackCollisionCheck();
	DoorCollisionCheck();
	
	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);
			break;
		case GH_KirbyAbilityState::DrainAfter:
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Down);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Down);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Down);
			break;
		default:
			break;
		}	
	}

	if (false == RightGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R"))
		{
			GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	if (false == LeftGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	
	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
	{
		if (false == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")
			&& false == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DrainReady);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::JumpUp);
		}

	}
		break;
	case GH_KirbyAbilityState::DrainAfter:
	{
		if (false == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")
			&& false == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			return GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Stand);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			return GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::JumpUp);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Attack);
		}

		if (true == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision)) //땅바닥에 닿아있을때만 아래키를 눌러 Eat상태로 들어갈 수 잇다
		{
			if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("D"))
			{
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Eat);
			}
		}
	}
		break;
	case GH_KirbyAbilityState::Beam:
	{
		if (false == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")
			&& false == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			return GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Stand);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			return GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::JumpUp);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Attack);
		}
	}
		break;
	case GH_KirbyAbilityState::Cutter:
	{
		if (false == GH_GameEngineInput::GH_GetInst().GH_IsPress("R")
			&& false == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			return GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Stand);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("Jump"))
		{
			return GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::JumpUp);
		}

		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
		{
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Attack);
		}
	}
		break;
	default:
		break;
	}
}



void Normal_Kirby::GH_DrainReady()
{
	GH_LeftRightCheck();
	MonsterCollisionCheck();
	MonsterAttackCollisionCheck();

	

	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		//뒷배경과의 충돌이 false라면
		GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
	}

	if (true == MainRender_->GH_IsCurAnimationEnd())
	{
		return GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DrainING);
	}
}

void Normal_Kirby::GH_DrainING()
{
	//밑의 충돌체크에서 ChageState하면서 소리가 바뀌므로 소리는 무조건 맨 위에 위치해야 한다.
	EffectPlayer->GH_PlayAlone("KirbyDrain.wav", 0);
	EffectPlayer->GH_PlayCountReset(1);

	DrainCollisionCheck();
	MonsterCollisionCheck();
	GH_LeftRightCheck();
	MonsterAttackCollisionCheck();

	
	
	if (AbilityType_ == GH_KirbyAbilityState::Normal)
	{
		DrainAttackCollision->GH_SetSize({ 150,70 });
	}
	else
	{
		return;
	}
	

	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		//뒷배경과의 충돌이 false라면
		GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
	}

	if (false == GH_GameEngineInput::GH_GetInst().GH_IsPress("Att") && false == DrainMonsterActive)
	{
		EffectPlayer->GH_Stop();
		EffectPlayer->GH_PlayCountReset(1);
		DrainAttackCollision->GH_SetSize({ 0,0 });
		return GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
	}
}

void Normal_Kirby::GH_JumpUp()
{
	EffectPlayer->GH_PlayAlone("KirbyJump.wav", 0);
	EffectPlayer->GH_PlayCountReset(1);

	GH_LeftRightCheck();
	MonsterCollisionCheck();
	MonsterAttackCollisionCheck();

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Jump"))
	{
		

		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Fly);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Fly);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Fly);
			break;
		default:
			break;
		}
	}

	if (false == RightGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R"))
		{
			GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	if (false == LeftGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DrainReady);
			break;
		case GH_KirbyAbilityState::DrainAfter:
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Attack);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Attack);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Attack);
			break;
		default:
			break;
		}

	}



	if (0 >= JumpDir_.y)//점프력이 남아있으면
	{
		JumpDir_ += GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 1500.0f;
		if (false == TopGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
		{
			GH_SetMove(JumpDir_ * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime());
		}
		
	
		if (0 < JumpDir_.y)//점프의 가장 높은 점에 도달하면
		{
			
			switch (AbilityType_)
			{
			case GH_KirbyAbilityState::Normal:
				GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::JumpDown);
				break;
			case GH_KirbyAbilityState::DrainAfter:
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::JumpDown);
				break;
			case GH_KirbyAbilityState::Beam:
				GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::JumpDown);
				break;
			case GH_KirbyAbilityState::Cutter:
				GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::JumpDown);
				break;
			default:
				break;
			}
		}
		return;
	}


}

void Normal_Kirby::GH_JumpDown()
{
	GH_LeftRightCheck();
	MonsterCollisionCheck();
	MonsterAttackCollisionCheck();


	if (false == RightGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R"))
		{
			GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	if (false == LeftGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}



	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		//뒷배경과의 충돌이 false라면
		GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);	
	}


	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Jump"))
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Fly);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Fly);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Fly);
			break;
		default:
			break;
		}
	}


	switch (AbilityType_)
	{
		case GH_KirbyAbilityState::Normal:
			if (MainRender_->GH_IsCurAnimationEnd())
			{
				GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);
			}
			break;
		case GH_KirbyAbilityState::DrainAfter:
			if (DrainAfterRender_->GH_IsCurAnimationEnd())
			{
				GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Down);
			}
			break;
		case GH_KirbyAbilityState::Beam:
			if (BeamRender_->GH_IsCurAnimationEnd())
			{
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Down);
			}
			break;
		case GH_KirbyAbilityState::Cutter:
			if (CutterRender_->GH_IsCurAnimationEnd())
			{
				GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Down);
			}
			break;
		default:
			break;
	}
		

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DrainReady);
			break;
		case GH_KirbyAbilityState::DrainAfter:
			GH_ChangeState(GH_KirbyAbilityState::DrainAfter, GH_PlayerState::Attack);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Attack);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Attack);
			break;
		default:
			break;
		}

	}


}
 

void Normal_Kirby::GH_DamageUp()
{ 
	EffectPlayer->GH_PlayAlone("KirbyDamage.wav", 0);
	if (0 >= HP)
	{
		GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Dead);
		return;
	}

	
	BodyCheckCollision->GH_SetSize({ 0,0 });
	DrainAttackCollision->GH_SetSize({ 0,0 });
	DrainCheckCollision->GH_SetSize({ 0,0 });
	
	GH_float4 RightHit = { -1,-1 };
	GH_float4 LeftHit = { 1, -1 };
	DamageTime -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();

	if (dirstate_ == GH_DirectionState::RIGHT)
	{
		GH_SetMove(RightHit * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 60.0f);
	}
	else
	{
		GH_SetMove(LeftHit * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 60.0f);
	}
	
	if (0 >= DamageTime)
	{
		//데미지를 받을시 무조건 노말상태로 돌아가게한다
		EffectPlayer->GH_PlayCountReset(1);
		DamageTime = 0.5f;
		GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::DamageDown);
	}

}

void Normal_Kirby::GH_DamageDown()
{
	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		//뒷배경과의 충돌이 false라면
		GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
	}

	if (MainRender_->GH_IsCurAnimationEnd())
	{
		BodyCheckCollision->GH_SetSize({ 75,70 });
		GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);
	}
}

void Normal_Kirby::GH_Fly()
{
	EffectPlayer->GH_PlayOverLap("KirbyFly.wav");

	//플라이 애니메이션을 다시 반복시키고 싶다
	GH_LeftRightCheck();
	MonsterCollisionCheck();
	MonsterAttackCollisionCheck();

	if (false == RightGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("R"))
		{
			GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	if (false == LeftGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsPress("L"))
		{
			GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Att"))
	{
		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::Normal:
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::FlyEnd);
			break;
		case GH_KirbyAbilityState::Beam:
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::FlyEnd);
			break;
		case GH_KirbyAbilityState::Cutter:
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::FlyEnd);
			break;
		default:
			break;
		}
	}


	if (0 >= FlyDir_.y)//점프력이 남아있으면
	{
		//점프력을 일정하게 맞추기 위해선 여기에도 델타타임을 곱해주자
		FlyDir_ += GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 1000.0f;


		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Jump"))
		{
			EffectPlayer->GH_PlayCountReset(1);
			FlyDir_ = GH_float4::UP * 400.0f;
		}

		if (false == TopGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
		{
			GH_SetMove(FlyDir_ * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime());
		}


	}
	
	if (0 < FlyDir_.y) //점프의 가장 높은 점에 도달하면
	{
		if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Jump"))
		{
			EffectPlayer->GH_PlayCountReset(1);

			if (true == TopGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
			{
				FlyDir_ = { 0,0 };
			}
			else if (false == TopGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
			{
				switch (AbilityType_)
				{
				case GH_KirbyAbilityState::Normal:
					MainRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString(), true);
					break;
				case GH_KirbyAbilityState::Beam:
					BeamRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString(), true);
					break;
				case GH_KirbyAbilityState::Cutter:
					CutterRender_->GH_ChangeAnimation(AbilityStateName_ + StateName_ + GH_GetDirString(), true);
					break;
				default:
					break;
				}
				

				FlyDir_ = GH_float4::UP * 400.0f;
			}
		}


		if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
		{
			//뒷배경과의 충돌이 false라면
			GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 300.0f);
		}
		else
		{
			switch (AbilityType_)
			{
			case GH_KirbyAbilityState::Normal:
				GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::FlyEnd);
				break;
			case GH_KirbyAbilityState::Beam:
				GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::FlyEnd);
				break;
			case GH_KirbyAbilityState::Cutter:
				GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::FlyEnd);
				break;
			default:
				break;
			}

		}
	}
}

void Normal_Kirby::GH_FlyEnd()
{
	EffectPlayer->GH_PlayAlone("KirbyYawnAttack.wav");

	MonsterCollisionCheck();
	MonsterAttackCollisionCheck();

	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		if (MainRender_->GH_IsCurAnimationEnd())
		{
			EffectPlayer->GH_PlayCountReset(1);
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Down);

		}
		break;
	case GH_KirbyAbilityState::Beam:
		if (BeamRender_->GH_IsCurAnimationEnd())
		{
			EffectPlayer->GH_PlayCountReset(1);
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Down);
		}
		break;
	case GH_KirbyAbilityState::Cutter:
		if (CutterRender_->GH_IsCurAnimationEnd())
		{
			EffectPlayer->GH_PlayCountReset(1);
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Down);
		}
		break;
	default:
		break;
	}

	
}

void Normal_Kirby::GH_Attack()
{
	

	MonsterCollisionCheck();
	BeamCollisionCheck();
	MonsterAttackCollisionCheck();

	if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
	{
		//뒷배경과의 충돌이 false라면
		GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
	}

	if (false == ShootMoveActive && AbilityType_ != GH_KirbyAbilityState::Beam) //빔 상태에서는 bullet을 만들지 않는다
	{
		

		//총알을 발사한다
		Bullet* BulletPtr = Normal_Kirby::GH_Actor::GH_GetLevel()->GH_CreateActor<Bullet>();

		if (dirstate_ == GH_DirectionState::LEFT)
		{
			BulletPtr->BulletDir_ = BulletDirectionState::LEFT;
		}
		else
		{
			BulletPtr->BulletDir_ = BulletDirectionState::RIGHT;
		}

		switch (AbilityType_)
		{
		case GH_KirbyAbilityState::DrainAfter:
			BulletPtr->GH_ChangeState(GH_BulletState::Star);
			EffectPlayer->GH_PlayAlone("KirbyStarShot.wav");
			break;
		case GH_KirbyAbilityState::Cutter:
			BulletPtr->GH_ChangeState(GH_BulletState::Cutter);
			EffectPlayer->GH_PlayAlone("KirbyCutterAttack.wav");
			break;
		default:
			break;
		}
		
		BulletPtr->GH_SetPos(GH_GetPos());
		//커비 위치를 기준으로 총알을 만듬

		ShootMoveActive = true;
	}

	if (ShootMoveActive == false && AbilityType_ == GH_KirbyAbilityState::Beam)
	{
		//빔상태일때의 공격
		BeamAttackCollision->GH_SetSize({ 200,100 });
		EffectPlayer->GH_PlayOverLap("KirbyBeamAttack.wav", 0);

		ShootMoveActive = true;
	}

	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::DrainAfter:
		if (DrainAfterRender_->GH_IsCurAnimationEnd())
		{
			EffectPlayer->GH_PlayCountReset(1);
			ShootMoveActive = false;
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
		}
		break;
	case GH_KirbyAbilityState::Beam:
		if (BeamRender_->GH_IsCurAnimationEnd())
		{
			EffectPlayer->GH_PlayCountReset(1);
			ShootMoveActive = false;
			BeamAttackCollision->GH_SetSize({ 0,0 });
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Stand);
		}
		break;
	case GH_KirbyAbilityState::Cutter:
		if (CutterRender_->GH_IsCurAnimationEnd())
		{
			EffectPlayer->GH_PlayCountReset(1);
			ShootMoveActive = false;
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Stand);
		}
		break;
	default:
		break;
	}	
}

void Normal_Kirby::GH_Eat()
{
	

	switch (HoldingAbilityStarType_)
	{
	case GH_KirbyAbilityState::Normal:
	{
		EffectPlayer->GH_PlayOverLap("KirbyDrainEat.wav", 0);
		if (true == DrainAfterRender_->GH_IsCurAnimationEnd())
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Stand);
			EffectPlayer->GH_PlayCountReset(1);
		}
	}
		break;
	case GH_KirbyAbilityState::Beam:
	{
		
		EffectPlayer->GH_PlayOverLap("KirbyDrainChange.wav",0);
		if (true == DrainAfterRender_->GH_IsCurAnimationEnd())	
		{
			GH_ChangeState(GH_KirbyAbilityState::Beam, GH_PlayerState::Stand);
			EffectPlayer->GH_PlayCountReset(1);
		}
	}
		break;
	case GH_KirbyAbilityState::Cutter:
	{
		EffectPlayer->GH_PlayOverLap("KirbyDrainChange.wav", 0);
		if (true == DrainAfterRender_->GH_IsCurAnimationEnd())
		{
			GH_ChangeState(GH_KirbyAbilityState::Cutter, GH_PlayerState::Stand);
			EffectPlayer->GH_PlayCountReset(1);
		}
	}
		break;
	default:
		break;
	}

	
}

void Normal_Kirby::GH_Dead()
{
	Stage_one_one::StagePtr->BackgroundPlayer->GH_Stop();

	DamageTime -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();

	if (DamageTime < 0)
	{

		EffectPlayer->GH_PlayAlone("Die.wav");
	;
		MainRender_->GH_ChangeAnimation("NormalSpin");
		JumpDir_ += GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * 2000.0f;
		GH_SetMove(JumpDir_ * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime());

		if (false == EffectPlayer->GH_IsPlay() && true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Jump"))
		{
			GH_GetLevel()->StageRestart();
		}
	}

}

void Normal_Kirby::GH_DanceReady()
{
	int KirbyPosX = GH_GetPos().ix();
	int DancePosX = GH_GameEngineWindow::GH_GetInst().GH_GetSize().GH_halffloat4().ix() - 100;

	if (KirbyPosX != DancePosX)
	{
		if ((KirbyPosX - DancePosX) < 0)
		{
			MainRender_->GH_ChangeAnimation("NormalWalkR");
			GH_SetMove(GH_float4::RIGHT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
		else
		{
			MainRender_->GH_ChangeAnimation("NormalWalkL");
			GH_SetMove(GH_float4::LEFT * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
	}
	else
	{
		if (false == MiddleGroundCheckCollision->GH_CollisionCheck(One_One_Back::MapCollision))
		{
			MainRender_->GH_ChangeAnimation("NormalJumpDownR");
			GH_SetMove(GH_float4::DOWN * GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime() * Speed);
		}
		else
		{
			GH_ChangeState(GH_KirbyAbilityState::Normal, GH_PlayerState::Dance);
		}
	
	}

}


void Normal_Kirby::GH_Dance()
{

	DamageTime -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();

	if (DamageTime <= 0)
	{
		EffectPlayer->GH_PlayAlone("KirbyDance.wav");


		if (DanceRender_->GH_IsCurAnimationEnd() && true == DanceRender_->GH_IsOn())
		{
			if (true == DanceRender_->GH_IsCurAnimationName("KirbyDanceWait"))
			{
				DanceRender_->GH_ChangeAnimation("KirbyDance");
				DanceRendertwo_->GH_Off();
				//DanceRendertwo_->GH_ChangeAnimation("KirbyDanceWaittwo");
				DanceRenderthree_->GH_Off();
				//DanceRenderthree_->GH_ChangeAnimation("KirbyDanceWaitthree");

			}
			else
			{
				DanceRender_->GH_Off();
				DanceRendertwo_->GH_On();
				//DanceRendertwo_->GH_ChangeAnimation("KirbyDanceWaittwo");
				DanceRenderthree_->GH_Off();

			}
		}

		if (DanceRendertwo_->GH_IsCurAnimationEnd() && true == DanceRendertwo_->GH_IsOn())
		{
			if (true == DanceRendertwo_->GH_IsCurAnimationName("KirbyDanceWaittwo"))
			{
				//DanceRender_->GH_ChangeAnimation("KirbyDanceWait");
				DanceRender_->GH_Off();
				DanceRendertwo_->GH_ChangeAnimation("KirbyDancetwo");
				//DanceRenderthree_->GH_ChangeAnimation("KirbyDanceWaitthree");
				DanceRenderthree_->GH_Off();

			}
			else
			{
				DanceRender_->GH_Off();
				DanceRendertwo_->GH_Off();
				DanceRenderthree_->GH_On();
				//DanceRenderthree_->GH_ChangeAnimation("KirbyDanceWaitthree");
			}
		}


		if (DanceRenderthree_->GH_IsCurAnimationEnd() && true == DanceRenderthree_->GH_IsOn())
		{
			if (true == DanceRenderthree_->GH_IsCurAnimationName("KirbyDanceWaitthree"))
			{
				DanceRender_->GH_Off();
				DanceRendertwo_->GH_Off();
				DanceRenderthree_->GH_ChangeAnimation("KirbyDancethree");
			}
		}

		

		if ((false == EffectPlayer->GH_IsPlay()) && (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Jump")))
		{
			Stage_one_one::StagePtr->ChangeStage();
			GH_LevelManager::GH_GetInst().GH_ChangeLevel("EndLevel");
		}
	}
}

void Normal_Kirby::GH_Render()
{
	if (DebugState_ == DEBUGMODE::DEBUGMODE)
	{
		MiddleGroundCheckCollision->GH_DebugRender();
		LeftGroundCheckCollision->GH_DebugRender();
		RightGroundCheckCollision->GH_DebugRender();
		TopGroundCheckCollision->GH_DebugRender();
		BodyCheckCollision->GH_DebugRender();
		DrainAttackCollision->GH_DebugRender();
		DrainCheckCollision->GH_DebugRender();
		BeamAttackCollision->GH_DebugRender();
	}
	

	switch (AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
	{
		
		if (stateType_ != GH_PlayerState::Dance)
		{
			MainRender_->GH_AnimationUpdate();
		
		}
		else if (stateType_ == GH_PlayerState::Dance)
		{
			if (DanceRender_ ->GH_IsOn())
			{
				DanceRender_->GH_AnimationUpdate();
			}

			if (DanceRendertwo_->GH_IsOn())
			{
				DanceRendertwo_->GH_AnimationUpdate();
			}

			if (DanceRenderthree_->GH_IsOn())
			{
				DanceRenderthree_->GH_AnimationUpdate();
			}
		}


		if (stateType_ == GH_PlayerState::DrainING)
		{
			DrainEffectRender_->GH_AnimationUpdate();
		}

		if (stateType_ == GH_PlayerState::DamageUp)
		{
			EatEffectRender_->GH_AnimationUpdate();
		}
	}
		break;
	case GH_KirbyAbilityState::DrainAfter:
		DrainAfterRender_->GH_AnimationUpdate();
		if (stateType_ == GH_PlayerState::Eat && HoldingAbilityStarType_ != GH_KirbyAbilityState::Normal)
		{
			EatEffectRender_->GH_AnimationUpdate();
		}
		
		break;
	case GH_KirbyAbilityState::Beam:
		BeamRender_->GH_AnimationUpdate();
		if (stateType_ == GH_PlayerState::Attack)
		{
			BeamEffectRender_->GH_AnimationUpdate();
		}
		break;
	case GH_KirbyAbilityState::Cutter:
		CutterRender_->GH_AnimationUpdate();
		break;
	default:
		break;
	}
	
}

