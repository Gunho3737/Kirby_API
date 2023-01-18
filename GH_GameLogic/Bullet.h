#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"

//분류 :
//용도 :
//설명 :
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
private: //디폴트 접근 지정자

	
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
	
	//총알이 파괴될때의 렌더
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
	Bullet& operator=(const Bullet& _Other) = delete; //디폴트 대입연산자
	Bullet& operator=(Bullet&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	Bullet(); //디폴트 생성자
	~Bullet(); //디폴트 소멸자
public:
	Bullet(const Bullet& _Other) = delete; //디폴트 복사 생성자
	Bullet(Bullet&& _Other) noexcept; //디폴트 RValue 복사생성자

private:
	void MonsterCollisionCheck();
	void KirbyCollisionCheck();
public:
	void GH_ChangeState(GH_BulletState _StateType);

};

