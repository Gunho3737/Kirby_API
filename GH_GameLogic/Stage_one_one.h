#pragma once
#include <GH_Level.h>
#include "GameLogicEnum.h"



//분류 :
//용도 :
//설명 :
class One_One_Back;
class One_One_Far;
class Normal_Kirby;
class WeakMonster;
class BeamMonster;
class CutterMonster;
class GH_GameEngineSoundPlayer;
class InGame_UI;
class TreeBoss;
class StartLevel;
class Stage_one_one : public GH_Level
{
	friend InGame_UI;
	friend TreeBoss;
	friend Normal_Kirby;
	friend StartLevel;

private: //디폴트 접근 지정자

	GH_GameEngineSoundPlayer* BackgroundPlayer;
	StageState CurStage;
	
public:
	Stage_one_one& operator=(const Stage_one_one& _Other) = delete; //디폴트 대입연산자
	Stage_one_one& operator=(Stage_one_one&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	Stage_one_one(); //디폴트 생성자
	~Stage_one_one(); //디폴트 소멸자
	Stage_one_one(const Stage_one_one& _Other) = delete; //디폴트 복사 생성자
	Stage_one_one(Stage_one_one&& _Other) noexcept; //디폴트 RValue 복사생성자
public:
	void GH_Loading() override;
	//부모 클래스인 Level의 Loading대신 실행

	void ChangeStage() override;

	void StageRestart() override;

	void GH_LevelUpdate() override;

private:
	static Normal_Kirby* NormalKirbyPtr;
	WeakMonster* WeakMonsterPtr;
	BeamMonster* BeamMonsterPtr;
	CutterMonster* CutterMonsterPtr;
	One_One_Far* FarPtr;
	One_One_Back* BackPtr;
	InGame_UI* UIPtr;
	static TreeBoss* TreeBossPtr;
	static Stage_one_one* StagePtr;
	
};

