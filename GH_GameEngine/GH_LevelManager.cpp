#include "GH_LevelManager.h"

#include "GH_ResourcesManager.h"
#include "GH_GameEngineCollision.h"
#include <GH_GameEngineTime.h>

GH_LevelManager* GH_LevelManager::Instance = new GH_LevelManager();
//맵의 인자를 동적할당으로 만듬


GH_LevelManager::GH_LevelManager()
	: LevelMap_(), CurrentLevel_(nullptr), NextLevel_(nullptr)
{

}

GH_LevelManager::~GH_LevelManager()
{
	//소멸자에서 map을 전부 delete 해줘야한다
	std::map<std::string, GH_Level*>::iterator StartIter = LevelMap_.begin();
	std::map<std::string, GH_Level*>::iterator EndIter = LevelMap_.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	LevelMap_.clear();
	//맵에 있는 모든 원소를 삭제

}

GH_LevelManager::GH_LevelManager(GH_LevelManager&& _Other) noexcept
{
}



void GH_LevelManager::GH_ChangeLevel(std::string _LVname)
{
	std::map<std::string, GH_Level*>::iterator FindIter = LevelMap_.find(_LVname);

	if (LevelMap_.end() == FindIter)
	{
		GH_GameEngineDebug::Assert();
		return;
		//기존과 같은레벨로 바꾸려고 할 경우 터진다
	}

	GH_GameEngineCollision::GH_ColCheckFuncInitialize();
	//모든 충돌이 담기는 이차원 배열에 전부 nullptr을 넣어줌

	NextLevel_ = FindIter->second;
	//NextLevel_을 nullptr에서 들어온 Level로 바꿔줌

}

void GH_LevelManager::GH_Update()
{
	if (nullptr != NextLevel_)
	{
		//ChangeLevel로 인해 다음 레벨이 존재한다면

		//맨 최초, currentLevel이 없다면
		if (nullptr == CurrentLevel_)
		{
			CurrentLevel_ = NextLevel_;
		}
		else
		{
			//이전 레벨이 있다면
			CurrentLevel_ = NextLevel_;
		}

		NextLevel_ = nullptr;
		GH_GameEngineTime::GH_GetInst().GH_TimeCheckReset();
		//레벨의 변경은 프레임이 시작되기전에 체크하는것이 가장 안전함
	}


	// 업데이트 오더체크는 업데이트를 하기전에
	// 실행순서가 바뀌는 액터를 체크해서 순서를 변경해주는 작업을 합니다.
	CurrentLevel_->GH_UpdateOrderCheck();
	CurrentLevel_->GH_RenderOrderCheck();
	CurrentLevel_->GH_CollisionOrderCheck();
	
	CurrentLevel_->GH_LevelUpdateBefore();
	CurrentLevel_->GH_UpdateBefore();

	CurrentLevel_->GH_LevelUpdate();
	CurrentLevel_->GH_Update();

	CurrentLevel_->GH_LevelUpdateAfter();
	CurrentLevel_->GH_UpdateAfter();

	
	CurrentLevel_->GH_Render();

	GH_ResourcesManager::GH_GetInst().DoubleBuffering();
	//진짜 윈도우창에 복사를 받은 윈도우창을 붙혀넣는다

	CurrentLevel_->GH_Collision();
	//렌더와 업데이트가 끝나고 충돌을 확인

	CurrentLevel_->GH_Release();
	//모든 과정이 끝나고 Release여부를 확인

	//현재레벨을 계속해서 돌게한다
}