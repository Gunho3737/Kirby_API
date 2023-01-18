#pragma once
#include <GH_Level.h>

//�з� :
//�뵵 :
//���� :
class GH_GameEngineSoundPlayer;
class StartLevel : public GH_Level
{
private: //����Ʈ ���� ������
	GH_GameEngineSoundPlayer* BackGroundPlayer;
	
	
	
public:
	StartLevel& operator=(const StartLevel& _Other) = delete; //����Ʈ ���Կ�����
	StartLevel& operator=(StartLevel&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	StartLevel(); //����Ʈ ������
	~StartLevel(); //����Ʈ �Ҹ���
	StartLevel(const StartLevel& _Other) = delete; //����Ʈ ���� ������
	StartLevel(StartLevel&& _Other) noexcept; //����Ʈ RValue ���������

public:
	void GH_Loading() override;
	//�θ� Ŭ������ Level�� Loading��� ����

	void GH_LevelUpdate() override;
	
};

