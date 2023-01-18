#pragma once
#include "GH_GameEngineSound.h"
#include "GH_GameEngineSoundFile.h"
//�з� :
//�뵵 :
//���� :

class GH_GameEngineSoundPlayer
{
	friend GH_GameEngineSound;

private: //����Ʈ ���� ������
	GH_GameEngineSound* PlaySoundFile_;

	FMOD::Channel* PlayChannel_;
	//������ ���/������ ����ϴ� FMOD �������̽�

	int PlayCount;
	//�Ҹ� �ݺ�Ƚ���� ����
	//����Ŀ��� PlayCountReset���� Ƚ���� �ٽ� �ʱ�ȭ ��������Ѵ�
	
public:
	GH_GameEngineSoundPlayer& operator=(const GH_GameEngineSoundPlayer& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineSoundPlayer& operator=(GH_GameEngineSoundPlayer&& _Other) = delete; //����Ʈ RValue ���Կ�����
private:
	GH_GameEngineSoundPlayer(); //����Ʈ ������
	~GH_GameEngineSoundPlayer(); //����Ʈ �Ҹ���
public:
	GH_GameEngineSoundPlayer(const GH_GameEngineSoundPlayer& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineSoundPlayer(GH_GameEngineSoundPlayer&& _Other) noexcept; //����Ʈ RValue ���������
	
public:
	bool GH_IsPlay();


	void GH_PlayCountReset(int _count = 1);

	//���带 ���ļ� ����Ұ�� ����ϴ� �Լ�
	void GH_PlayOverLap(const std::string& _name, int LoopCount = 0);

	//���带 ���ļ� �������� ���ϰ� �ϰ� ������ �ϳ��� ����ɼ� �ִ� ���� ����Լ�
	void GH_PlayAlone(const std::string& _name, int LoopCount = 0);


	void GH_Stop();
};

