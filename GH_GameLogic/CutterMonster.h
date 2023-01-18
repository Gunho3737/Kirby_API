#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"


//�з� :
//�뵵 :
//���� :

class GH_GameEngineCollision;
class GH_GameEngineSoundPlayer;
class CutterMonster : public GH_Actor
{
private: //����Ʈ ���� ������
	GH_MonsterDirectionState dirstate_;
	std::string StateName_;
	std::string MonsterName_;
	GH_MonsterState stateType_;
	DEBUGMODE MonsterDebugState_;
private:
	bool ShootMoveActive;

public:
	GH_Renderer* MainRender_;
	GH_Renderer* DestroyRender_;
	static float TimeCheck;
	static int Movement;
private:
	GH_GameEngineCollision* BodyCheckCollision;
	GH_GameEngineSoundPlayer* EffectPlayer;

public:
	CutterMonster& operator=(const CutterMonster& _Other) = delete; //����Ʈ ���Կ�����
	CutterMonster& operator=(CutterMonster&& _Other) = delete; //����Ʈ RValue ���Կ�����

public:
	CutterMonster(); //����Ʈ ������
	~CutterMonster(); //����Ʈ �Ҹ���
	CutterMonster(const CutterMonster& _Other) = delete; //����Ʈ ���� ������
	CutterMonster(CutterMonster&& _Other) noexcept; //����Ʈ RValue ���������
	
public:
	void GH_LeftRightCheck();

public:
	void GH_AttackReady();
	void GH_Damage();
	void GH_Dead();
	void GH_GetDrain();
	void GH_Jump();
	void GH_Attack();

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
public:

public:
	void GH_ChangeState(GH_MonsterState _StateType);
	std::string GH_GetDirString();
};

