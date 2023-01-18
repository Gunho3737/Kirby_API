#include "GH_LevelManager.h"

#include "GH_ResourcesManager.h"
#include "GH_GameEngineCollision.h"
#include <GH_GameEngineTime.h>

GH_LevelManager* GH_LevelManager::Instance = new GH_LevelManager();
//���� ���ڸ� �����Ҵ����� ����


GH_LevelManager::GH_LevelManager()
	: LevelMap_(), CurrentLevel_(nullptr), NextLevel_(nullptr)
{

}

GH_LevelManager::~GH_LevelManager()
{
	//�Ҹ��ڿ��� map�� ���� delete ������Ѵ�
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
	//�ʿ� �ִ� ��� ���Ҹ� ����

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
		//������ ���������� �ٲٷ��� �� ��� ������
	}

	GH_GameEngineCollision::GH_ColCheckFuncInitialize();
	//��� �浹�� ���� ������ �迭�� ���� nullptr�� �־���

	NextLevel_ = FindIter->second;
	//NextLevel_�� nullptr���� ���� Level�� �ٲ���

}

void GH_LevelManager::GH_Update()
{
	if (nullptr != NextLevel_)
	{
		//ChangeLevel�� ���� ���� ������ �����Ѵٸ�

		//�� ����, currentLevel�� ���ٸ�
		if (nullptr == CurrentLevel_)
		{
			CurrentLevel_ = NextLevel_;
		}
		else
		{
			//���� ������ �ִٸ�
			CurrentLevel_ = NextLevel_;
		}

		NextLevel_ = nullptr;
		GH_GameEngineTime::GH_GetInst().GH_TimeCheckReset();
		//������ ������ �������� ���۵Ǳ����� üũ�ϴ°��� ���� ������
	}


	// ������Ʈ ����üũ�� ������Ʈ�� �ϱ�����
	// ��������� �ٲ�� ���͸� üũ�ؼ� ������ �������ִ� �۾��� �մϴ�.
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
	//��¥ ������â�� ���縦 ���� ������â�� �����ִ´�

	CurrentLevel_->GH_Collision();
	//������ ������Ʈ�� ������ �浹�� Ȯ��

	CurrentLevel_->GH_Release();
	//��� ������ ������ Release���θ� Ȯ��

	//���緹���� ����ؼ� �����Ѵ�
}