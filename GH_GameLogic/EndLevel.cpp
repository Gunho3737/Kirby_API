#include "EndLevel.h"
#include "EndScreen.h"
#include <GH_ResourcesManager.h>
#include <GH_GameEngineDirectory.h>
#include <GH_GameEngineSoundPlayer.h>

EndLevel::EndLevel()
{

}

EndLevel::~EndLevel()
{
}

EndLevel::EndLevel(EndLevel&& _Other) noexcept
{
}

void EndLevel::GH_Loading()
{
	BackgroundPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	GH_CreateActor<EndScreen>();
	//내 레벨에 쓸 Actor를 가져옴
	BackgroundPlayer->GH_PlayCountReset(1);

}

void EndLevel::GH_LevelUpdate()
{
	BackgroundPlayer->GH_PlayAlone("Ending.wav");
}