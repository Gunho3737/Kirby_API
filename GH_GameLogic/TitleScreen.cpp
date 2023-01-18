#include "TitleScreen.h"
#include <GH_GameEngineWindow.h>
#include <GH_GameEngineTime.h>
#include <GH_GameEngineImage.h>
#include <GH_ResourcesManager.h>
#include <GH_Renderer.h>
#include <GH_GameEngineInput.h>
#include <GH_LevelManager.h>

float TitleScreen::TimeCheck_ = 0.02f;

TitleScreen::TitleScreen()
{
	GH_SetPos(GH_GameEngineWindow::GH_GetInst().GH_GetSize().GH_halffloat4());
	//SetPos로 위치를

	GH_SetRenderOrder(1);
	//SetRenderOrder로 렌더링 순서를 정함
	
}

TitleScreen::~TitleScreen()
{
}

TitleScreen::TitleScreen(TitleScreen&& _Other) noexcept
{
}


void TitleScreen::GH_Update()
{
	if (StartRender_->GH_IsCurAnimationEnd())
	{
		TimeCheck_ -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	}

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Start"))
	{
		GH_LevelManager::GH_GetInst().GH_ChangeLevel("Stage_one_one");
	}


	if (TimeCheck_ < 0)
	{
		TimeCheck_ = 0.02f;
		StartRender_->GH_ChangeAnimation("PressEnter", true);
	}

}


void TitleScreen::GH_Render()
{
	
	MainScreen_->GH_Render();
	StartRender_->GH_AnimationUpdate();

}

void TitleScreen::GH_Start()
{
	if (false == GH_GameEngineInput::GH_GetInst().GH_IsKey("Start")) //"Start"라는 이름을 가진 키가 없다면
	{
		GH_GameEngineInput::GH_GetInst().GH_CreateKey("Start", VK_RETURN);
	}

	MainScreen_ = GH_CreateRenderer("Title_Screen.bmp");

	StartRender_ = GH_CreateRenderer("Press_Enter.bmp");
	StartRender_->GH_CreateAnimation("PressEnter","Press_Enter.bmp",0, 29, false, 0.03f);
	StartRender_->GH_SetPivotPos({0,325});
	StartRender_->GH_ChangeAnimation("PressEnter", true);

}