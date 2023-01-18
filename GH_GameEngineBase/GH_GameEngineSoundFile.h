#pragma once
#include "GH_GameEngineSound.h"
#include "GH_GameEngineNameBase.h"

//�з� :
//�뵵 :
//���� :

class GH_GameEngineSoundFile : public GH_GameEngineNameBase
{
	friend GH_GameEngineSound;
	friend GH_GameEngineSoundPlayer;

private: //����Ʈ ���� ������
	FMOD::Sound* Sound_;
	
public:
	bool GH_Load(const std::string& _path);
	
public:
	GH_GameEngineSoundFile& operator=(const GH_GameEngineSoundFile& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineSoundFile& operator=(GH_GameEngineSoundFile&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_GameEngineSoundFile(); //����Ʈ ������
	~GH_GameEngineSoundFile(); //����Ʈ �Ҹ���
	GH_GameEngineSoundFile(const GH_GameEngineSoundFile& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineSoundFile(GH_GameEngineSoundFile&& _Other) noexcept; //����Ʈ RValue ���������
	
};

