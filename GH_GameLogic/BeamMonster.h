#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"


//�з� :
//�뵵 :
//���� :

class GH_GameEngineSoundPlayer;
class GH_GameEngineCollision;
class BeamMonster : public GH_Actor
{
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
	static int Movement;

private:
	GH_GameEngineCollision* BodyCheckCollision;
	GH_GameEngineCollision* BeamAttackCollision;

public:
	BeamMonster& operator=(const BeamMonster& _Other) = delete; //����Ʈ ���Կ�����
	BeamMonster& operator=(BeamMonster&& _Other) = delete; //����Ʈ RValue ���Կ�����

public:
	BeamMonster(); //����Ʈ ������
	~BeamMonster(); //����Ʈ �Ҹ���
	BeamMonster(const BeamMonster& _Other) = delete; //����Ʈ ���� ������
	BeamMonster(BeamMonster&& _Other) noexcept; //����Ʈ RValue ���������
	
public:
	void GH_LeftRightCheck();

public:
	void GH_Walk();
	void GH_Damage();
	void GH_Dead();
	void GH_GetDrain();
	void GH_Attack();
	void GH_AttackReady();

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
public:

public:
	void GH_ChangeState(GH_MonsterState _StateType);
	std::string GH_GetDirString();
};

