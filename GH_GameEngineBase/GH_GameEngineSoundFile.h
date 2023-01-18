#pragma once
#include "GH_GameEngineSound.h"
#include "GH_GameEngineNameBase.h"

//분류 :
//용도 :
//설명 :

class GH_GameEngineSoundFile : public GH_GameEngineNameBase
{
	friend GH_GameEngineSound;
	friend GH_GameEngineSoundPlayer;

private: //디폴트 접근 지정자
	FMOD::Sound* Sound_;
	
public:
	bool GH_Load(const std::string& _path);
	
public:
	GH_GameEngineSoundFile& operator=(const GH_GameEngineSoundFile& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineSoundFile& operator=(GH_GameEngineSoundFile&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_GameEngineSoundFile(); //디폴트 생성자
	~GH_GameEngineSoundFile(); //디폴트 소멸자
	GH_GameEngineSoundFile(const GH_GameEngineSoundFile& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineSoundFile(GH_GameEngineSoundFile&& _Other) noexcept; //디폴트 RValue 복사생성자
	
};

