#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"


//분류 :
//용도 :
//설명 :
class Stage_one_one;
class GH_GameEngineCollision;
class GH_GameEngineSoundPlayer;
class WeakMonster : public GH_Actor
{
	friend Stage_one_one;

private: //디폴트 접근 지정자
	GH_MonsterDirectionState dirstate_;
	std::string StateName_;
	std::string MonsterName_;
	GH_MonsterState stateType_;
	DEBUGMODE MonsterDebugState_;
	GH_GameEngineSoundPlayer* EffectPlayer;

public:
	GH_Renderer* MainRender_;
	GH_Renderer* DestroyRender_;
	static float TimeCheck;

	//이동을 위해서 
	static int Movement;
private:
	GH_GameEngineCollision* BodyCheckCollision;

public:
	WeakMonster& operator=(const WeakMonster& _Other) = delete; //디폴트 대입연산자
	WeakMonster& operator=(WeakMonster&& _Other) = delete; //디폴트 RValue 대입연산자

public:
	WeakMonster(); //디폴트 생성자
	~WeakMonster(); //디폴트 소멸자
	WeakMonster(const WeakMonster& _Other) = delete; //디폴트 복사 생성자
	WeakMonster(WeakMonster&& _Other) noexcept; //디폴트 RValue 복사생성자
	
public:
	void GH_LeftRightCheck();

public:
	void GH_Walk();
	void GH_Damage();
	void GH_Dead();
	void GH_GetDrain();

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
public:

public:
	void GH_ChangeState(GH_MonsterState _StateType);
	std::string GH_GetDirString();
};

