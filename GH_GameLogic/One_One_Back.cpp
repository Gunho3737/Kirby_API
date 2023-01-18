#include "One_One_Back.h"
#include <GH_GameEngineImage.h>
#include <GH_ResourcesManager.h>
#include <GH_GameEngineCollision.h>
#include <GH_GameEngineInput.h>
#include <GH_Renderer.h>
#include <GH_GameEngineWindow.h>
#include <GH_LevelManager.h>

GH_GameEngineCollision* One_One_Back::MapCollision = nullptr;
GH_GameEngineCollision* One_One_Back::DoorCollision = nullptr;

One_One_Back::One_One_Back()
	: MapState(DEBUGMODE::PLAYMODE), One_One_ImageSize_({0,0}), StageMapState(StageState::ONEONE)
{
	GH_SetRenderOrder(2);
}

One_One_Back::~One_One_Back()
{
}

One_One_Back::One_One_Back(One_One_Back&& _Other) noexcept
{
}

void One_One_Back::GH_Update()
{
	if (MapState == DEBUGMODE::DEBUGMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		MapState = DEBUGMODE::PLAYMODE;
	}
	else if (MapState == DEBUGMODE::PLAYMODE && GH_GameEngineInput::GH_GetInst().GH_IsDown("ChangeDebugMode"))
	{
		MapState = DEBUGMODE::DEBUGMODE;
	}

	if (true == GH_GameEngineInput::GH_GetInst().GH_IsDown("Start"))
	{
		GH_LevelManager::GH_GetInst().GH_ChangeLevel("EndLevel");
	}
}


void One_One_Back::GH_Render()
{


	switch (StageMapState)
	{
	case StageState::ONEONE:
	{
		if (MapState == DEBUGMODE::DEBUGMODE)
		{
			One_One_DebugRender_->GH_Render();
			DoorCollision->GH_DebugRender();
		}
		else if (MapState == DEBUGMODE::PLAYMODE)
		{
			One_One_MainRender_->GH_Render();
		}
	}
		break;
	case StageState::ONETWO:
	{
		if (MapState == DEBUGMODE::DEBUGMODE)
		{
			One_Two_DebugRender_->GH_Render();
			DoorCollision->GH_DebugRender();
		}
		else if (MapState == DEBUGMODE::PLAYMODE)
		{
			One_Two_MainRender_->GH_Render();
		}
	}
		break;
	case StageState::BOSS:
	{
		if (MapState == DEBUGMODE::DEBUGMODE)
		{
			Boss_DebugRender_->GH_Render();
		}
		else if (MapState == DEBUGMODE::PLAYMODE)
		{
			Boss_MainRender_->GH_Render();
		}
	}
		break;
	default:
		break;
	}


//	GH_GameEngineImage* FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("1-1_big.bmp");
//	GH_GameEngineImage* BackImage = GH_ResourcesManager::GH_GetInst().GH_GetBackBufferImage();
//	BackImage->GH_BitCopyToImageSize(FindImage, GH_GetCamEffectPos());


}


void One_One_Back::GH_Start()
{
	GH_GameEngineImage* FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("1-1_big.bmp");
	One_One_ImageSize_ = FindImage->GH_GetSize();
	FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("1-2.bmp");
	One_Two_ImageSize_ = FindImage->GH_GetSize();
	FindImage = GH_ResourcesManager::GH_GetInst().GH_FindGameImage("Boss_BackGround.bmp");
	Boss_ImageSize_ = FindImage->GH_GetSize();


	MapCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MAP, GH_CollisionCheckType::IMAGE);
	MapCollision->GH_SetImage("Bit_1-1.bmp");

	One_One_MainRender_ = GH_CreateRenderer("1-1_big.bmp");
	One_One_DebugRender_ = GH_CreateRenderer("Bit_1-1.bmp");

	One_Two_MainRender_ = GH_CreateRenderer("1-2.bmp");
	One_Two_DebugRender_ = GH_CreateRenderer("Bit_1-2.bmp");

	Boss_MainRender_ = GH_CreateRenderer("Boss_BackGround.bmp");
	Boss_DebugRender_ = GH_CreateRenderer("Bit_Boss.bmp");

	DoorCollision = GH_CreateCollision(GAMECOLLISIONGROUP::MAP, GH_CollisionCheckType::RECTANGLE);
	DoorCollision->GH_SetSize({100,100});


	//이미지의 중앙이 0,0이 되는 것이므로 이미지의 크기의 절반만큼 pivot을 더해줘야함
	One_One_MainRender_->GH_SetPivotPos(One_One_ImageSize_.GH_halffloat4());
	One_One_DebugRender_->GH_SetPivotPos(One_One_ImageSize_.GH_halffloat4());
	One_Two_MainRender_->GH_SetPivotPos(One_Two_ImageSize_.GH_halffloat4());
	One_Two_DebugRender_->GH_SetPivotPos(One_Two_ImageSize_.GH_halffloat4());
	Boss_MainRender_->GH_SetPivotPos(Boss_ImageSize_.GH_halffloat4());
	Boss_DebugRender_->GH_SetPivotPos(Boss_ImageSize_.GH_halffloat4());




}