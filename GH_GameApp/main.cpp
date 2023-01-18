#include <iostream>
#include <Windows.h>
#include <GH_GameEngineWindow.h>
#include <GH_GameEngineDebug.h>
#include <GH_LevelManager.h>
#include <StartLevel.h>
#include <Stage_one_one.h>
#include <EndLevel.h>
#include <GH_GameEngineMath.h>
#include <GH_GameEngineTime.h>
#include <GH_ResourcesManager.h>
#include "GH_AppFunc.h"
#include <GH_GameEngineSound.h>

void GH_UpdateFunc()
{
	GH_LevelManager::GH_GetInst().GH_Update();
}

//winapi의 진입점
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	//동적할당 후 릭이 남아있아있으면 알려준다
	GH_GameEngineDebug::GH_LeakCheckOn();

	GH_GameEngineSound::GH_GetInst().GH_SoundInit();

	// 윈도우 생성및 기본 윈도우 창에서 게임을 돌릴 준비를함
	GH_GameEngineWindow::GH_GetInst().GH_CreateWindowClass(hInstance, "GameWindow");
	GH_GameEngineWindow::GH_GetInst().GH_CreateMainWindow("Kirby", { 1024, 768 }, { 0,0 });

	//리소스 로드
	GH_AppResourcesInit();

	//각 레벨 생성
	//내 포트폴리오에 필요한 레벨들을 만들어야 한다
	GH_LevelManager::GH_GetInst().GH_CreateLevel<StartLevel>("StartLevel");
	GH_LevelManager::GH_GetInst().GH_CreateLevel<Stage_one_one>("Stage_one_one");
	GH_LevelManager::GH_GetInst().GH_CreateLevel<EndLevel>("EndLevel");

	//최초 레벨을 정함
	GH_LevelManager::GH_GetInst().GH_ChangeLevel("StartLevel");

	//현재 레벨을 계속해서 루프한다
	GH_GameEngineWindow::GH_GetInst().GH_Loop(GH_UpdateFunc);

	GH_AppRelease();



}


