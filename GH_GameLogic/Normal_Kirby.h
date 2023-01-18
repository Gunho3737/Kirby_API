#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"
//�з� :
//�뵵 :
//���� :



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
	//����� ���Ͱ� ���� �������� string���� ����

	GH_float4 PlayerPivot_;
private:
	DEBUGMODE DebugState_;

public:

	GH_DirectionState GH_GetDirState()
	{
		return dirstate_;
	}

public: //����Ʈ ���� ������

	//�⺻ Ŀ���� �ִϸ��̼ǿ� ��������
	GH_Renderer* MainRender_;

	//������ ���� Ŀ�� ���� ����
	GH_Renderer* DanceRender_;
	GH_Renderer* DanceRendertwo_;
	GH_Renderer* DanceRenderthree_;

	//����� �� ���¿����� �⺻Ŀ���� �ִϸ��̼ǿ� ��������
	GH_Renderer* DrainAfterRender_;
	//���Ƶ��϶��� ����Ʈ��
	GH_Renderer* DrainEffectRender_;

	//Ŀ�ͻ��¿����� �ִϸ��̼ǿ� ���� ����
	GH_Renderer* CutterRender_;

	//�����¿����� �ִϸ��̼ǿ� ���� ����
	GH_Renderer* BeamRender_;
	//�� ä�� ������ ����Ʈ��
	GH_Renderer* BeamEffectRender_;
	//��ų�� ����Ʈ��
	GH_Renderer* EatEffectRender_;
	
	GH_GameEngineCollision* MiddleGroundCheckCollision;
	GH_GameEngineCollision* LeftGroundCheckCollision;
	GH_GameEngineCollision* RightGroundCheckCollision;
	GH_GameEngineCollision* TopGroundCheckCollision;


	GH_GameEngineCollision* BodyCheckCollision;
	GH_GameEngineCollision* DrainAttackCollision; //Ŀ���� ��������� ����
	GH_GameEngineCollision* DrainCheckCollision; //���������� ������ �ޱ����� ����
	GH_GameEngineCollision* BeamAttackCollision; //��ä�� ������ ���� ����

	//ȿ���� ����� ���� �÷��̾�
	GH_GameEngineSoundPlayer* EffectPlayer;

private:
	static int BackGroundSizeforCamX;
	//ī�޶� �̵� ������ ���ؼ�
	//�޹���� �� �̹����� Xũ�⸦ �޾ƿ��� ���� int

	static float DamageTime;
	//������ ���¿��� �ִϸ��̼��� �Ѿ�� �ð��� ��� ���� float

	static bool DrainMonsterActive;
	//���͸� ����ϱ� ���������� �߰������� �������� bool

	static bool ShootMoveActive;
	//���� �ִϸ��̼� ����߿� �߰������� �������� bool
	
public:
	Normal_Kirby& operator=(const Normal_Kirby& _Other) = delete; //����Ʈ ���Կ�����
	Normal_Kirby& operator=(Normal_Kirby&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	Normal_Kirby(); //����Ʈ ������
	~Normal_Kirby(); //����Ʈ �Ҹ���
	Normal_Kirby(const Normal_Kirby& _Other) = delete; //����Ʈ ���� ������
	Normal_Kirby(Normal_Kirby&& _Other) noexcept; //����Ʈ RValue ���������
	

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;

public:
	//���� ���� Ŀ����¸� üũ�ϴ� enum����
	GH_KirbyAbilityState AbilityType_;
	
	//DrainAfter���¿��� �Կ� ���� �ִ� �� �������� üũ�ϴ� enum����
	GH_KirbyAbilityState HoldingAbilityStarType_;

	//���� Ŀ���� �ൿ ���¸� ��Ÿ���� ����
	GH_PlayerState stateType_;

	//������ �ڿ������� �ϰ� ������ִ� float4
	//�����Ӹ��� DOWN -= �� �ؼ�, ��� �������� �ö󰡴°� ���߰� ����� �ִ�
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