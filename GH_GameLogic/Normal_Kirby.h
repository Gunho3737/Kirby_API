#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"
//분류 :
//용도 :
//설명 :



enum class GH_PlayerState
{
	Stand,
	Down,
	Walk,
	Run,
	JumpUp,
	JumpDown,
	Fly,
	FlyEnd,
	DrainReady,
	DrainING,
	DamageUp,
	DamageDown,
	Attack,
	Eat,
	Dead,
	Dance,
	DanceReady,
	Max

};

enum class GH_DirectionState
{
	LEFT,
	RIGHT,
};

class GH_GameEngineSoundPlayer;
class Normal_Kirby : public GH_Actor
{
public:
	int HP;


private:
	float Speed;
	
	GH_DirectionState dirstate_;
	std::string AbilityStateName_;
	std::string StateName_;

	std::string DrainedMonsterState_;
	//흡수한 몬스터가 무슨 몬스터인지 string으로 저장

	GH_float4 PlayerPivot_;
private:
	DEBUGMODE DebugState_;

public:

	GH_DirectionState GH_GetDirState()
	{
		return dirstate_;
	}

public: //디폴트 접근 지정자

	//기본 커비의 애니메이션용 렌더변수
	GH_Renderer* MainRender_;

	//엔딩씬 직전 커비 댄스용 변수
	GH_Renderer* DanceRender_;
	GH_Renderer* DanceRendertwo_;
	GH_Renderer* DanceRenderthree_;

	//흡수를 한 상태에서의 기본커비의 애니메이션용 렌더변수
	GH_Renderer* DrainAfterRender_;
	//빨아들일때의 이펙트용
	GH_Renderer* DrainEffectRender_;

	//커터상태에서의 애니메이션용 렌더 변수
	GH_Renderer* CutterRender_;

	//빔상태에서의 애니메이션용 렌더 변수
	GH_Renderer* BeamRender_;
	//빔 채찍 공격의 이펙트용
	GH_Renderer* BeamEffectRender_;
	//삼킬때 이펙트용
	GH_Renderer* EatEffectRender_;
	
	GH_GameEngineCollision* MiddleGroundCheckCollision;
	GH_GameEngineCollision* LeftGroundCheckCollision;
	GH_GameEngineCollision* RightGroundCheckCollision;
	GH_GameEngineCollision* TopGroundCheckCollision;


	GH_GameEngineCollision* BodyCheckCollision;
	GH_GameEngineCollision* DrainAttackCollision; //커비의 흡수공격의 판정
	GH_GameEngineCollision* DrainCheckCollision; //끌려들어오는 적들을 받기위한 판정
	GH_GameEngineCollision* BeamAttackCollision; //빔채찍 공격을 위한 판정

	//효과음 재생용 사운드 플레이어
	GH_GameEngineSoundPlayer* EffectPlayer;

private:
	static int BackGroundSizeforCamX;
	//카메라 이동 제한을 위해서
	//뒷배경이 될 이미지의 X크기를 받아오기 위한 int

	static float DamageTime;
	//데미지 상태에서 애니메이션이 넘어가는 시간을 재기 위한 float

	static bool DrainMonsterActive;
	//몬스터를 흡수하기 시작했을때 추가조작을 막기위한 bool

	static bool ShootMoveActive;
	//공격 애니메이션 재생중에 추가조작을 막기위한 bool
	
public:
	Normal_Kirby& operator=(const Normal_Kirby& _Other) = delete; //디폴트 대입연산자
	Normal_Kirby& operator=(Normal_Kirby&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	Normal_Kirby(); //디폴트 생성자
	~Normal_Kirby(); //디폴트 소멸자
	Normal_Kirby(const Normal_Kirby& _Other) = delete; //디폴트 복사 생성자
	Normal_Kirby(Normal_Kirby&& _Other) noexcept; //디폴트 RValue 복사생성자
	

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;

public:
	//실제 현재 커비상태를 체크하는 enum변수
	GH_KirbyAbilityState AbilityType_;
	
	//DrainAfter상태에서 입에 물고 있는 게 무엇인지 체크하는 enum변수
	GH_KirbyAbilityState HoldingAbilityStarType_;

	//현재 커비의 행동 상태를 나타내는 변수
	GH_PlayerState stateType_;

	//점프를 자연스럽게 하게 만들어주는 float4
	//프레임마다 DOWN -= 를 해서, 어느 지점에서 올라가는게 멈추게 만들수 있다
	GH_float4 JumpDir_;
	GH_float4 FlyDir_;

public:
	void GH_StartStand();
	void GH_StartDown();
	void GH_StartWalk();
	void GH_StartDrainReady();
	void GH_StartDrainING();
	void GH_StartJumpUp();
	void GH_StartJumpDown();
	void GH_StartDamageUp();
	void GH_StartDamageDown();
	void GH_StartFly();
	void GH_StartFlyEnd();
	void GH_StartAttack();
	void GH_StartEat();
	void GH_StartDead();
	void GH_StartDanceReady();
	void GH_StartDance();

	void GH_Stand();
	void GH_Down();
	void GH_Walk();
	void GH_DrainReady();
	void GH_DrainING();
	void GH_JumpUp();
	void GH_JumpDown();
	void GH_DamageUp();
	void GH_DamageDown();
	void GH_Fly();
	void GH_FlyEnd();
	void GH_Attack();
	void GH_Eat();
	void GH_Dead();
	void GH_DanceReady();
	void GH_Dance();

	void GH_LeftRightCheck();
	void MonsterCollisionCheck();
	void DrainCollisionCheck();
	void MonsterAttackCollisionCheck();
	void BeamCollisionCheck();
	void DoorCollisionCheck();

	void MakeBreath();

	std::string GH_GetDirString();
	std::string GH_GetAbilityString();



	
public:
	void GH_ChangeState(GH_KirbyAbilityState _AbilityType, GH_PlayerState _StateType);
};