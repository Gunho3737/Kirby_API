#include "GH_Actor.h"
#include "GH_Renderer.h"
#include "GH_Level.h"
#include "GH_GameEngineCollision.h"

GH_Actor::GH_Actor()
	: ActorNumber(-1),
	UpdateOrder_(0),
	RenderOrder_(0),
	ParentLevel_(nullptr)
{

}

GH_Actor::~GH_Actor()
{
	{
		std::list<GH_Renderer*>::iterator StartIter = RendererList_.begin();
		std::list<GH_Renderer*>::iterator EndIter = RendererList_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr == (*StartIter))
			{
				continue;
			}
			delete (*StartIter);
			(*StartIter) = nullptr;
		}
		RendererList_.clear();
	}
	
	{
		std::list<GH_GameEngineCollision*>::iterator StartIter = CollisionList_.begin();
		std::list<GH_GameEngineCollision*>::iterator EndIter = CollisionList_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr == (*StartIter))
			{
				continue;
			}
			delete (*StartIter);
			(*StartIter) = nullptr;
		}
		CollisionList_.clear();
	}

}

GH_float4 GH_Actor::GH_GetCamEffectPos()
{
	return GH_GetPos() - ParentLevel_->GH_GetCamPos();
}

GH_Actor::GH_Actor(GH_Actor&& _Other) noexcept
{
}


void GH_Actor::GH_UpdateBefore()
{
}

void GH_Actor::GH_Update()
{
}

void GH_Actor::GH_UpdateAfter()
{
}

void GH_Actor::GH_Render()
{
}

void GH_Actor::GH_Start()
{
}

void GH_Actor::GH_Collision()
{
}


GH_Renderer* GH_Actor::GH_CreateRenderer(std::string _ImageName)
{
	GH_Renderer* NewRenderer = new GH_Renderer();
	NewRenderer->GH_SetActor(this);
	//Renderer의 부모인 Actor도 들어온 이미지에 대한 정보를 가지게 만듬
	NewRenderer->GH_SetActor(this);
	NewRenderer->GH_SetImage(_ImageName);
	RendererList_.push_back(NewRenderer);

	return NewRenderer;
}

GH_GameEngineCollision* GH_Actor::GH_CreateCollision(int _Group, GH_CollisionCheckType _type)
{
	GH_GameEngineCollision* NewCollision = new GH_GameEngineCollision();

	NewCollision->GH_SetParent(this);
	//Actor도 collision에 대한 정보를 가지게 만듬
	NewCollision->GH_SetActor(this);
	//충돌이 만들어지고 있는 액터는 지금 이 액터
	NewCollision->GH_SetColType(_type);
	NewCollision->GH_SetGroupIndex(_Group);
	CollisionList_.push_back(NewCollision);
	GH_GetLevel()->GH_PushCollision(NewCollision);
	return NewCollision;
}