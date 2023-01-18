#pragma once
#include <FMOD/fmod.hpp>
#include <map>
#include <string>
#include <list>

#pragma comment(lib, "FMOD/fmod_vc.lib") //Release�� ����� �ڵ�����

//�з� :
//�뵵 :
//���� :

class GH_GameEngineSoundFile;
class GH_GameEngineSoundPlayer;
class GH_GameEngineSound
{
	friend GH_GameEngineSoundFile;
	friend GH_GameEngineSoundPlayer;

private:
	static GH_GameEngineSound* Instance;

public:
	static GH_GameEngineSound& GH_GetInst()
	{
		return *Instance;
	}

	static void GH_Destroy()
	{
		if (nullptr != Instance)
		{
			delete Instance;
			Instance = nullptr;
		}

	}

private: //����Ʈ ���� ������
	FMOD::System* SoundSystem_; //Fmod�� �����ϴ� �������̽�
	//SoundSystem�� �������� �ϳ� �ϳ��� �ƴ϶� fmod�� ��뿩�θ� Ȯ�����ְ�
	//���带 �ε��ϰ� ���� �� �ִ� �⺻ �������̽�
	
	std::map<std::string, GH_GameEngineSoundFile*> AllLoadSound_;

	std::list<GH_GameEngineSoundPlayer*> AllSoundPlayer_;

private:
	GH_GameEngineSoundFile* GH_FindSound(const std::string& _name);

public:
	void GH_SoundInit();
	void GH_LoadSound(const std::string& _name, const std::string& _path);
	GH_GameEngineSoundPlayer* GH_CreateSoundPlayer();

public:
	void GH_SoundUpdate();

public:
	GH_GameEngineSound& operator=(const GH_GameEngineSound& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineSound& operator=(GH_GameEngineSound&& _Other) = delete; //����Ʈ RValue ���Կ�����
private:
	GH_GameEngineSound(); //����Ʈ ������
	~GH_GameEngineSound(); //����Ʈ �Ҹ���
public:
	GH_GameEngineSound(const GH_GameEngineSound& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineSound(GH_GameEngineSound&& _Other) noexcept; //����Ʈ RValue ���������
	
};

