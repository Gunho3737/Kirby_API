#include "EndScreen.h"
#include <GH_GameEngineWindow.h>
#include <GH_GameEngineTime.h>
#include <GH_GameEngineImage.h>
#include <GH_ResourcesManager.h>
#include <GH_Renderer.h>
#include <GH_GameEngineInput.h>
#include <GH_LevelManager.h>

EndScreen::EndScreen()
{
	GH_SetPos(GH_GameEngineWindow::GH_GetInst().GH_GetSize().GH_halffloat4());
	//SetPos�� ��ġ��

	GH_SetRenderOrder(1);
	//SetRenderOrder�� ������ ������ ����
}

EndScreen::~EndScreen()
{
}

EndScreen::EndScreen(EndScreen&& _Other) noexcept
{
}

void EndScreen::GH_Update()
{

}

void EndScreen::GH_Render()
{
	MainScreen_->GH_Render();

}

void EndScreen::GH_Start()
{
	
	MainScreen_ = GH_CreateRenderer("Complete_.bmp");
}