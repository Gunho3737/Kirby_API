#include "One_One_Far.h"
#include <GH_GameEngineImage.h>
#include <GH_ResourcesManager.h>
#include <GH_GameEngineCollision.h>




One_One_Far::One_One_Far()
{
	GH_SetRenderOrder(1);
	MapState = StageState::ONEONE;
}

One_One_Far::~One_One_Far()
{
}

One_One_Far::One_One_Far(One_One_Far&& _Other) noexcept
{
}

void One_One_Far::GH_Update()
{
	
}


void One_One_Far::GH_Render()
{
	switch (MapState)
	{
	case StageState::ONEONE:
	{
		GH_GameEngineImage* FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("1-1_background.bmp");
		GH_GameEngineImage* BackImage = GH_ResourcesManager::GH_GetInst().GH_GetBackBufferImage();
		//ī�޶��� �̵��� 0.5�踸 �ް� �����ν� �ָ��ִ� ����� ������ �����δ�
		BackImage->GH_BitCopyToImageSize(FindImage, (GH_GetCamEffectPos()) * 0.5f);
	}
		break;
	case StageState::ONETWO:
	{
		GH_GameEngineImage* FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("1-2_background.bmp");
		GH_GameEngineImage* BackImage = GH_ResourcesManager::GH_GetInst().GH_GetBackBufferImage();
		//ī�޶��� �̵��� 0.5�踸 �ް� �����ν� �ָ��ִ� ����� ������ �����δ�
		BackImage->GH_BitCopyToImageSize(FindImage, (GH_GetCamEffectPos()) * 0.5f);
	}
		break;
	case StageState::BOSS:
	{
		GH_GameEngineImage* FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("1-2_background.bmp");
		GH_GameEngineImage* BackImage = GH_ResourcesManager::GH_GetInst().GH_GetBackBufferImage();
		//ī�޶��� �̵��� 0.5�踸 �ް� �����ν� �ָ��ִ� ����� ������ �����δ�
		BackImage->GH_BitCopyToImageSize(FindImage, (GH_GetCamEffectPos()) * 0.5f);
	}
		break;
	default:
		break;
	}

}


void One_One_Far::GH_Start()
{
}