#include "GH_GameEngineCollision.h"
#include <GH_GameEngineDebug.h>
#include "GH_ResourcesManager.h"
#include "GH_Actor.h"
#include "GH_Level.h"
#include "GH_GameEngineImage.h"


GH_GameEngineCollision::GH_GameEngineCollision()
	: collisionType_(GH_CollisionCheckType::MAX)
	, GroupIndex_(0)
	, isCamEffect_(true)
{
}

GH_GameEngineCollision::~GH_GameEngineCollision()
{
}

GH_GameEngineCollision::GH_GameEngineCollision(GH_GameEngineCollision&& _Other) noexcept
{
}

GH_float4 GH_GameEngineCollision::GH_GetCollisionPos()
{
	return Actor_->GH_GetPos() + Pivot_;
}

bool GH_GameEngineCollision::GH_CollisionCheck(GH_GameEngineCollision* _other)
{
	if (nullptr == this)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	if (nullptr == _other)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	if (nullptr == GH_arrFunc_[GH_GetTypeToIndex()][_other->GH_GetTypeToIndex()])
	{
		GH_GameEngineDebug::MessageBoxError("구현하지 않은 충돌함수를 이용하려고 한다.");
		return false;
	}

	return GH_arrFunc_[GH_GetTypeToIndex()][_other->GH_GetTypeToIndex()](this, _other);
}

void GH_GameEngineCollision::GH_SetImage(std::string _ImageName)
{
	GH_GameEngineImage* ImagePtr = GH_ResourcesManager::GH_GetInst().GH_FindGameImage(_ImageName);

	if (nullptr == ImagePtr)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	ImagePtr_ = ImagePtr;
}

void GH_GameEngineCollision::GH_DebugRender()
{
	//충돌박스를 시각적으로 보여주는 함수
	
	GH_GameEngineImage* BackBufferImage = GH_ResourcesManager::GH_GetInst().GH_GetBackBufferImage();

	GH_float4 ActorPos = Actor_->GH_GetCamEffectPos();

	if (false == isCamEffect_)
	{
		ActorPos = Actor_->GH_GetPos();
	}

	GH_float4 RenderPos = ActorPos + Pivot_;

	switch (collisionType_)
	{
	case GH_CollisionCheckType::IMAGE:
		break;
	case GH_CollisionCheckType::POINT:
	{
		Rectangle(BackBufferImage->GH_GetDC(),
			RenderPos.ix() - 5,
			RenderPos.iy() - 5,
			RenderPos.ix() + 5,
			RenderPos.iy() + 5);
	}
		break;
	case GH_CollisionCheckType::RECTANGLE:
	{
		Rectangle(BackBufferImage->GH_GetDC(),
			RenderPos.ix() - Size_.ihx(),
			RenderPos.iy() - Size_.ihy(),
			RenderPos.ix() + Size_.ihx(),
			RenderPos.iy() + Size_.ihy());

	}
		break;
	case GH_CollisionCheckType::MAX:
		break;
	default:
		break;
	}
}

GH_Figure GH_GameEngineCollision::GH_GetFigure()
{
	if (false == isCamEffect_)
	{
		return { Actor_->GH_GetPos() + Pivot_, Size_ };
	}

	return { Actor_->GH_GetCamEffectPos() + Pivot_, Size_ };
}

GH_GameEngineCollision* GH_GameEngineCollision::GH_CollisionGroupCheckOne(int _otherIndex)
{
	if (nullptr == this)
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
	}

	if (nullptr == Actor_)
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
	}

	if (nullptr == Actor_->GH_GetLevel())
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
	}

	std::list<GH_GameEngineCollision*>& GroupList = Actor_->GH_GetLevel()->GH_GetCollisionList(_otherIndex);

	if (true == GroupList.empty())
	{
		return nullptr;
	}

	std::list<GH_GameEngineCollision*>::iterator StartIter = GroupList.begin();
	std::list<GH_GameEngineCollision*>::iterator EndIter = GroupList.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		if (true == GH_CollisionCheck(*StartIter))
		{
			return *StartIter;
		}
	}

	return nullptr;
}

//std::list<GH_GameEngineCollision> GH_GameEngineCollision::GH_ColligionGroupCheck(int _otherIndex)
//{
//
//}