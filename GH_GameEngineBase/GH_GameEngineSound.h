#pragma once
#include <FMOD/fmod.hpp>
#include <map>
#include <string>
#include <list>

#pragma comment(lib, "FMOD/fmod_vc.lib") //Release용 빌드된 코드파일

//분류 :
//용도 :
//설명 :

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

private: //디폴트 접근 지정자
	FMOD::System* SoundSystem_; //Fmod가 제공하는 인터페이스
	//SoundSystem은 사운드파일 하나 하나가 아니라 fmod의 사용여부를 확인해주고
	//사운드를 로드하게 해줄 수 있는 기본 인터페이스
	
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
	GH_GameEngineSound& operator=(const GH_GameEngineSound& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineSound& operator=(GH_GameEngineSound&& _Other) = delete; //디폴트 RValue 대입연산자
private:
	GH_GameEngineSound(); //디폴트 생성자
	~GH_GameEngineSound(); //디폴트 소멸자
public:
	GH_GameEngineSound(const GH_GameEngineSound& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineSound(GH_GameEngineSound&& _Other) noexcept; //디폴트 RValue 복사생성자
	
};

