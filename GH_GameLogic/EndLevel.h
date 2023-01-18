#pragma once
#include <GH_Level.h>

//�з� :
//�뵵 :
//���� :
class GH_GameEngineSoundPlayer;
class EndLevel : public GH_Level
{
private: //����Ʈ ���� ������
	GH_GameEngineSoundPlayer* BackgroundPlayer;
	
	
	
public:
	EndLevel& operator=(const EndLevel& _Other) = delete; //����Ʈ ���Կ�����
	EndLevel& operator=(EndLevel&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	EndLevel(); //����Ʈ ������
	~EndLevel(); //����Ʈ �Ҹ���
	EndLevel(const EndLevel& _Other) = delete; //����Ʈ ���� ������
	EndLevel(EndLevel&& _Other) noexcept; //����Ʈ RValue ���������

public:
	void GH_Loading() override;
	//�θ� Ŭ������ Level�� Loading��� ����

	void GH_LevelUpdate() override;
	
};

