#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"


//�з� :
//�뵵 :
//���� :
class Stage_one_one;
class GH_GameEngineCollision;
class GH_GameEngineSoundPlayer;
class WeakMonster : public GH_Actor
{
	friend Stage_one_one;

private: //����Ʈ ���� ������
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

	//�̵��� ���ؼ� 
	static int Movement;
private:
	GH_GameEngineCollision* BodyCheckCollision;

public:
	WeakMonster& operator=(const WeakMonster& _Other) = delete; //����Ʈ ���Կ�����
	WeakMonster& operator=(WeakMonster&& _Other) = delete; //����Ʈ RValue ���Կ�����

public:
	WeakMonster(); //����Ʈ ������
	~WeakMonster(); //����Ʈ �Ҹ���
	WeakMonster(const WeakMonster& _Other) = delete; //����Ʈ ���� ������
	WeakMonster(WeakMonster&& _Other) noexcept; //����Ʈ RValue ���������
	
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

