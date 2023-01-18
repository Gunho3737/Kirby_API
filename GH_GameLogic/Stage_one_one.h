#pragma once
#include <GH_Level.h>
#include "GameLogicEnum.h"



//�з� :
//�뵵 :
//���� :
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

private: //����Ʈ ���� ������

	GH_GameEngineSoundPlayer* BackgroundPlayer;
	StageState CurStage;
	
public:
	Stage_one_one& operator=(const Stage_one_one& _Other) = delete; //����Ʈ ���Կ�����
	Stage_one_one& operator=(Stage_one_one&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	Stage_one_one(); //����Ʈ ������
	~Stage_one_one(); //����Ʈ �Ҹ���
	Stage_one_one(const Stage_one_one& _Other) = delete; //����Ʈ ���� ������
	Stage_one_one(Stage_one_one&& _Other) noexcept; //����Ʈ RValue ���������
public:
	void GH_Loading() override;
	//�θ� Ŭ������ Level�� Loading��� ����

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

