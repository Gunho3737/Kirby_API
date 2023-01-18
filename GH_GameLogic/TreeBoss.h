#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"


//�з� :
//�뵵 :
//���� :

class GH_GameEngineCollision;
class GH_GameEngineSoundPlayer;
class TreeBoss : public GH_Actor
{
private: //����Ʈ ���� ������
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
	TreeBoss& operator=(const TreeBoss& _Other) = delete; //����Ʈ ���Կ�����
	TreeBoss& operator=(TreeBoss&& _Other) = delete; //����Ʈ RValue ���Կ�����

public:
	TreeBoss(); //����Ʈ ������
	~TreeBoss(); //����Ʈ �Ҹ���
	TreeBoss(const TreeBoss& _Other) = delete; //����Ʈ ���� ������
	TreeBoss(TreeBoss&& _Other) noexcept; //����Ʈ RValue ���������
	
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

