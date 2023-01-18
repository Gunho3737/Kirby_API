#include "GH_GameEngineSoundPlayer.h"
#include "GH_GameEngineSoundFile.h"
#include "GH_GameEngineDebug.h"

GH_GameEngineSoundPlayer::GH_GameEngineSoundPlayer()
	: PlaySoundFile_(nullptr)
	, PlayChannel_(nullptr)
	, PlayCount(-1)
{

}

GH_GameEngineSoundPlayer::~GH_GameEngineSoundPlayer()
{
}

GH_GameEngineSoundPlayer::GH_GameEngineSoundPlayer(GH_GameEngineSoundPlayer&& _Other) noexcept
{
}

void GH_GameEngineSoundPlayer::GH_PlayCountReset(int _Count)
{
	//단발성 사운드 출력후에 다시 
	PlayCount = _Count;
}

bool GH_GameEngineSoundPlayer::GH_IsPlay()
{
	bool Check = false;
	PlayChannel_->isPlaying(&Check);
	return Check;
	//FMOD의 isPlayIng함수로 Check값을 변화시킨다
}

void GH_GameEngineSoundPlayer::GH_PlayOverLap(const std::string& _name, int LoopCount/*= 0*/)
{
	GH_GameEngineSoundFile* Soundptr = GH_GameEngineSound::GH_GetInst().GH_FindSound(_name);

	if (nullptr == Soundptr)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 플레이 에러");
		return;
	}

	if (0 == PlayCount)
	{
		//정해진 횟수만큼 반복되고나면 실행 되지 않음
		return;
	}

	GH_GameEngineSound::GH_GetInst().SoundSystem_->playSound(
		Soundptr->Sound_
		, nullptr
		, false
		, &PlayChannel_
		);
	
	--PlayCount;

	PlayChannel_->setLoopCount(LoopCount);
}

void GH_GameEngineSoundPlayer::GH_PlayAlone(const std::string& _name, int LoopCount/*= 0*/)
{
	GH_GameEngineSoundFile* Soundptr = GH_GameEngineSound::GH_GetInst().GH_FindSound(_name);

	if (true == GH_IsPlay())
	{
		return;
		//사운드가 실행중이면 실행시키지 않는다
	}

	if (nullptr == Soundptr)
	{
		GH_GameEngineDebug::MessageBoxError("사운드 플레이 에러");
		return;
	}

	if (0 == PlayCount)
	{
		//정해진 횟수만큼 반복되고나면 실행 되지 않음
		return;
	}

	GH_GameEngineSound::GH_GetInst().SoundSystem_->playSound(
		Soundptr->Sound_
		, nullptr
		, false
		, &PlayChannel_
	);

	--PlayCount;

	PlayChannel_->setLoopCount(LoopCount);
}

void GH_GameEngineSoundPlayer::GH_Stop()
{
	if (nullptr == PlayChannel_)
	{
		return;
	}

	PlayChannel_->stop();
	PlayChannel_ = nullptr;
}