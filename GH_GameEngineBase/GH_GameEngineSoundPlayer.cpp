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
	//�ܹ߼� ���� ����Ŀ� �ٽ� 
	PlayCount = _Count;
}

bool GH_GameEngineSoundPlayer::GH_IsPlay()
{
	bool Check = false;
	PlayChannel_->isPlaying(&Check);
	return Check;
	//FMOD�� isPlayIng�Լ��� Check���� ��ȭ��Ų��
}

void GH_GameEngineSoundPlayer::GH_PlayOverLap(const std::string& _name, int LoopCount/*= 0*/)
{
	GH_GameEngineSoundFile* Soundptr = GH_GameEngineSound::GH_GetInst().GH_FindSound(_name);

	if (nullptr == Soundptr)
	{
		GH_GameEngineDebug::MessageBoxError("���� �÷��� ����");
		return;
	}

	if (0 == PlayCount)
	{
		//������ Ƚ����ŭ �ݺ��ǰ��� ���� ���� ����
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
		//���尡 �������̸� �����Ű�� �ʴ´�
	}

	if (nullptr == Soundptr)
	{
		GH_GameEngineDebug::MessageBoxError("���� �÷��� ����");
		return;
	}

	if (0 == PlayCount)
	{
		//������ Ƚ����ŭ �ݺ��ǰ��� ���� ���� ����
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