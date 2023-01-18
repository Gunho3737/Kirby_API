#include "GH_GameEngineSound.h"
#include "GH_GameEngineDebug.h"
#include "GH_GameEngineSoundFile.h"
#include "GH_GameEngineSoundPlayer.h"

GH_GameEngineSound* GH_GameEngineSound::Instance = new GH_GameEngineSound();

GH_GameEngineSound::GH_GameEngineSound()
{

}

GH_GameEngineSound::~GH_GameEngineSound()
{
	{
		std::list<GH_GameEngineSoundPlayer*>::iterator StartIter = AllSoundPlayer_.begin();
		std::list<GH_GameEngineSoundPlayer*>::iterator EndIter = AllSoundPlayer_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != *StartIter)
			{
				delete* StartIter;
			}
		}
		AllSoundPlayer_.clear();
	}

	{
		std::map<std::string, GH_GameEngineSoundFile*>::iterator StartIter = AllLoadSound_.begin();
		std::map<std::string, GH_GameEngineSoundFile*>::iterator EndIter = AllLoadSound_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != StartIter->second)
			{
				delete StartIter->second;
				StartIter->second = nullptr;
			}
		}
		AllLoadSound_.clear();
	}

	if (nullptr != SoundSystem_)
	{
		SoundSystem_->release();
		SoundSystem_ = nullptr;
	}

}

GH_GameEngineSound::GH_GameEngineSound(GH_GameEngineSound&& _Other) noexcept
{
}

GH_GameEngineSoundFile* GH_GameEngineSound::GH_FindSound(const std::string& _name)
{
	std::map<std::string, GH_GameEngineSoundFile*>::iterator FindIter = AllLoadSound_.find(_name);

	if (FindIter == AllLoadSound_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void GH_GameEngineSound::GH_SoundUpdate()
{
	if (nullptr == SoundSystem_)
	{
		GH_GameEngineDebug::MessageBoxError("SoundSystem is Nullptr");
		return;
	}

	SoundSystem_->update();
}

GH_GameEngineSoundPlayer* GH_GameEngineSound::GH_CreateSoundPlayer()
{
	GH_GameEngineSoundPlayer* NewSoundPlayer = new GH_GameEngineSoundPlayer();

	AllSoundPlayer_.push_back(NewSoundPlayer);

	return NewSoundPlayer;

}

void GH_GameEngineSound::GH_LoadSound(const std::string& _name, const std::string& _path)
{
	if (nullptr != GH_FindSound(_name))
	{
		GH_GameEngineDebug::MessageBoxError("사운드 중복 로드 에러");
		return;
	}

	GH_GameEngineSoundFile* NewLoadSound = new GH_GameEngineSoundFile();

	if (false == NewLoadSound->GH_Load(_path))
	{
		GH_GameEngineDebug::MessageBoxError("사운드 로드 에러");
		delete NewLoadSound;
		return;
	}

	AllLoadSound_.insert(
		std::map<std::string, GH_GameEngineSoundFile*>::value_type(_name, NewLoadSound));
}

void GH_GameEngineSound::GH_SoundInit()
{
	FMOD::System_Create(&SoundSystem_);

	if (nullptr == SoundSystem_)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 시스템 Create 에러");
		return;
	}

	//32채널 까지 가능
	if (FMOD_OK != SoundSystem_->init(32, FMOD_DEFAULT, nullptr))
	{
		GH_GameEngineDebug::MessageBoxError("사운드 시스템 이니셜라이저 에러");
		return;
	}
}