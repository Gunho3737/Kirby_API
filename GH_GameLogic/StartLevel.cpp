#include "StartLevel.h"
#include "TitleScreen.h"
#include <GH_ResourcesManager.h>
#include <GH_GameEngineDirectory.h>
#include <GH_GameEngineSoundPlayer.h>
#include <GH_GameEngineInput.h>
#include "Stage_one_one.h"

StartLevel::StartLevel()
{

}

StartLevel::~StartLevel()
{
}

StartLevel::StartLevel(StartLevel&& _Other) noexcept
{
}



void StartLevel::GH_Loading()
{
	
	BackGroundPlayer = GH_GameEngineSound::GH_GetInst().GH_CreateSoundPlayer();
	GH_CreateActor<TitleScreen>();
	//내 레벨에 쓸 Actor를 가져옴
}

void StartLevel::GH_LevelUpdate()
{
	BackGroundPlayer->GH_PlayAlone("Title.mp3", -1);

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Start"))
	{
		BackGroundPlayer->GH_Stop();
		Stage_one_one::StagePtr->StageRestart();
	}
}