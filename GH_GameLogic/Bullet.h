#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"

//�з� :
//�뵵 :
//���� :
enum class BulletDirectionState
{
	LEFT,
	RIGHT,
};

enum class GH_BulletState
{
	Star,
	Cutter,
	MonsterCutter,
	Apple,
	Breath,
	GetDrain,
	KirbyBreath,
	Dead
};

class GH_GameEngineSoundPlayer;
class Bullet : public GH_Actor
{
private: //����Ʈ ���� ������

	
public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;

public:
	GH_GameEngineCollision* BodyCheckCollision;
	GH_GameEngineCollision* WallCheckCollision;
	GH_GameEngineCollision* GroundCheckCollision;
	GH_Renderer* MainRender_;
	GH_Renderer* CutterRender_;
	GH_Renderer* MonsterCutterRender_;
	GH_Renderer* AppleRender_;
	GH_Renderer* BreathRender_;
	GH_Renderer* KirbyBreathRender_;
	
	//�Ѿ��� �ı��ɶ��� ����
	GH_Renderer* DestroyRender_;

	GH_GameEngineSoundPlayer* EffectPlayer;

public:
	DEBUGMODE BulletDebugState_;
	BulletDirectionState BulletDir_;
	GH_BulletState StateType_;

private:
	void Star();
	void Dead();
	void Cutter();
	void MonsterCutter();
	void Apple();
	void GetDrain();
	void Breath();
	void KirbyBreath();

	void CheckBulletType();

private:
	GH_float4 CutterDir_;

public:
	Bullet& operator=(const Bullet& _Other) = delete; //����Ʈ ���Կ�����
	Bullet& operator=(Bullet&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	Bullet(); //����Ʈ ������
	~Bullet(); //����Ʈ �Ҹ���
public:
	Bullet(const Bullet& _Other) = delete; //����Ʈ ���� ������
	Bullet(Bullet&& _Other) noexcept; //����Ʈ RValue ���������

private:
	void MonsterCollisionCheck();
	void KirbyCollisionCheck();
public:
	void GH_ChangeState(GH_BulletState _StateType);

};

