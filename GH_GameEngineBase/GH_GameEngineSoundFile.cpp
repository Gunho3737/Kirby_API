#include "GH_GameEngineSoundFile.h"
#include "GH_GameEngineDebug.h"

GH_GameEngineSoundFile::GH_GameEngineSoundFile()
	: Sound_(nullptr)
{

}

GH_GameEngineSoundFile::~GH_GameEngineSoundFile()
{
}

GH_GameEngineSoundFile::GH_GameEngineSoundFile(GH_GameEngineSoundFile&& _Other) noexcept
	: Sound_(_Other.Sound_)
{
}

bool GH_GameEngineSoundFile::GH_Load(const std::string& _path)
{
	if (FMOD_RESULT::FMOD_OK !=
		GH_GameEngineSound::GH_GetInst().SoundSystem_->createSound(
			_path.c_str(), FMOD_LOOP_NORMAL, nullptr, &Sound_))
	{
		GH_GameEngineDebug::MessageBoxError("사운드 로드 에러");
		return false;
	}

	if (nullptr == Sound_)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 포인터가 nullptr");
		return false;
	}

	return true;
}