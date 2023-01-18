#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"


//분류 :
//용도 :
//설명 :

class GH_GameEngineCollision;
class GH_GameEngineSoundPlayer;
class TreeBoss : public GH_Actor
{
private: //디폴트 접근 지정자
	std::string StateName_;
	std::string MonsterName_;
	GH_MonsterState stateType_;
	DEBUGMODE MonsterDebugState_;
	GH_GameEngineSoundPlayer* EffectPlayer;
public:
	GH_Renderer* MainRender_;
	GH_Renderer* EffectRender_;
	static float TimeCheck;
	static float AttackTimeCheck;
	int HP;
	bool KirbyDanceActive = false;
private:
	GH_GameEngineCollision* BodyCheckCollision;

public:
	TreeBoss& operator=(const TreeBoss& _Other) = delete; //디폴트 대입연산자
	TreeBoss& operator=(TreeBoss&& _Other) = delete; //디폴트 RValue 대입연산자

public:
	TreeBoss(); //디폴트 생성자
	~TreeBoss(); //디폴트 소멸자
	TreeBoss(const TreeBoss& _Other) = delete; //디폴트 복사 생성자
	TreeBoss(TreeBoss&& _Other) noexcept; //디폴트 RValue 복사생성자
	
public:
	int BreathCount;

public:
	void GH_Damage();
	void GH_Dead();
	void GH_AttackReady();
	void GH_Idle();
	void GH_Attack();
	void GH_Breath();

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
public:

public:
	void GH_ChangeState(GH_MonsterState _StateType);
};

