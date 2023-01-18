#include "GH_Level.h"
#include "GH_Actor.h"
#include "GH_GameEngineCollision.h"


GH_Level::GH_Level()
	: loadingnow_(false)
	, camPos_(GH_float4::ZERO)
{
	//0번 그룹을 만들어줌
	AllActorUpdateList_.insert(std::map<int, std::list<GH_Actor*>>::value_type(0, std::list<GH_Actor*>()));
	AllActorRenderList_.insert(std::map<int, std::list<GH_Actor*>>::value_type(0, std::list<GH_Actor*>()));
	AllActorCollisionList_.insert(std::map<int, std::list<GH_GameEngineCollision*>>::value_type(0, std::list<GH_GameEngineCollision*>()));
}

GH_Level::~GH_Level()
{
	std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
	std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
		std::list<GH_Actor*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			delete (*Start);
			(*Start) = nullptr;
		}
	}

	AllActorUpdateList_.clear();

}

GH_Level::GH_Level(GH_Level&& _Other) noexcept
{
}

void GH_Level::ChangeStage()
{
	//Stage_One_One에 넘겨주기 위한 가상함수
}

void GH_Level::StageRestart()
{
	//Stage_One_One에 넘겨주기 위한 가상함수
}

std::list<GH_GameEngineCollision*>& GH_Level::GH_GetCollisionList(int _Group)
{
	std::map<int, std::list<GH_GameEngineCollision*>>::iterator FindGroupIter
		= AllActorCollisionList_.find(_Group);

	if (FindGroupIter == AllActorCollisionList_.end())
	{
		GH_GameEngineDebug::Assert();
	}

	return FindGroupIter->second;
}

GH_Actor* GH_Level::GH_FindActor(const std::string& _actorName)
{
	std::map<std::string, GH_Actor*>::iterator FindIter = ActorMap_.find(_actorName);

	if (ActorMap_.end() == FindIter)
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
		//내가 찾는 key값을 찾는 actor가 없을경우 터트림
	}

	return FindIter->second;
	//찾았으면 그 key의 value값을 리턴
}



void GH_Level::GH_UpdateOrderCheck()
{
	{
		//  새로운 그룹을 만든다.
		std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
		std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();
		for (; UpdateStart != UpdateEnd; ++UpdateStart)
		{
			std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
			std::list<GH_Actor*>::iterator End = UpdateStart->second.end();
			for (; Start != End; ++Start)
			{
				// 이게 다르다는것은
				// 이녀석이 자신의 순서를 바꾸겠다는 이야기가 된다.
				if (UpdateStart->first == (*Start)->UpdateOrder_)
				{
					continue;
				}

				if (AllActorUpdateList_.end() == AllActorUpdateList_.find((*Start)->UpdateOrder_))
				{
					AllActorUpdateList_.insert(std::map<int, std::list<GH_Actor*>>::value_type((*Start)->UpdateOrder_, std::list<GH_Actor*>()));
				}

				std::map<int, std::list<GH_Actor*>>::iterator UpdateIter = AllActorUpdateList_.find((*Start)->UpdateOrder_);
				UpdateIter->second.push_back((*Start));
			}
		}
	}

	{
		// 지워주는 부분
		// 다른 녀석들을 지워준다.
		std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
		std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();
		for (; UpdateStart != UpdateEnd; ++UpdateStart)
		{
			std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
			std::list<GH_Actor*>::iterator End = UpdateStart->second.end();
			for (; Start != End; )
			{
				if (UpdateStart->first == (*Start)->UpdateOrder_)
				{
					++Start;
					continue;
				}

				Start = UpdateStart->second.erase(Start);
			}
		}
	}
}

void GH_Level::GH_UpdateBefore()
{
	std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
	std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
		std::list<GH_Actor*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->GH_UpdateBefore();
		}
	}

	//순회를 시켜서 모든 Actor들의 UpdateBefore을 실행시킨다

}

void GH_Level::GH_Update()
{
	std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
	std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
		std::list<GH_Actor*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->GH_Update();
		}
	}

	//순회를 시켜서 모든 Actor들의 Update를 실행시킨다
}

void GH_Level::GH_Collision()
{
	std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
	std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
		std::list<GH_Actor*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->GH_Collision();
		}
	}
}


void GH_Level::GH_UpdateAfter()
{
	std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
	std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
		std::list<GH_Actor*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->GH_UpdateAfter();
		}
	}

	//순회를 시켜서 모든 Actor들의 UpdateAfter를 실행시킨다
}

void GH_Level::GH_RenderOrderCheck()
{
	{
		//새로운 그룹을 만든다
		std::map<int, std::list<GH_Actor*>>::iterator RenderStart = AllActorRenderList_.begin();
		std::map<int, std::list<GH_Actor*>>::iterator RenderEnd = AllActorRenderList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GH_Actor*>::iterator Start = RenderStart->second.begin();
			std::list<GH_Actor*>::iterator End = RenderStart->second.end();
			for (; Start != End; ++Start)
			{
				if (RenderStart->first == (*Start)->RenderOrder_)
				{
					continue;
				}

				if (AllActorRenderList_.end() == AllActorRenderList_.find((*Start)->RenderOrder_))
				{
					AllActorRenderList_.insert(std::map<int, std::list<GH_Actor*>>::value_type((*Start)->RenderOrder_, std::list<GH_Actor*>()));
					//Update/Render의 Order의 값을 key로 가진 각각의 새로운 map을 만든다
				}

				std::map<int, std::list<GH_Actor*>>::iterator RenderIter = AllActorRenderList_.find((*Start)->RenderOrder_);
				RenderIter->second.push_back((*Start));
			}
		}
	}

	{
		std::map<int, std::list<GH_Actor*>>::iterator RenderStart = AllActorRenderList_.begin();
		std::map<int, std::list<GH_Actor*>>::iterator RenderEnd = AllActorRenderList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GH_Actor*>::iterator Start = RenderStart->second.begin();
			std::list<GH_Actor*>::iterator End = RenderStart->second.end();
			for (; Start != End; )
			{
				if (RenderStart->first == (*Start)->RenderOrder_)
				{
					++Start;
					continue;
				}

				Start = RenderStart->second.erase(Start);
			}
		}


	}
}

void GH_Level::GH_CollisionOrderCheck()
{
	{
		//새로운 그룹을 만든다
		std::map<int, std::list<GH_GameEngineCollision*>>::iterator RenderStart = AllActorCollisionList_.begin();
		std::map<int, std::list<GH_GameEngineCollision*>>::iterator RenderEnd = AllActorCollisionList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GH_GameEngineCollision*>::iterator Start = RenderStart->second.begin();
			std::list<GH_GameEngineCollision*>::iterator End = RenderStart->second.end();
			for (; Start != End; ++Start)
			{
				if (RenderStart->first == (*Start)->GH_GetGroupIndex())
				{
					continue;
				}

				if (AllActorCollisionList_.end() == AllActorCollisionList_.find((*Start)->GH_GetGroupIndex()))
				{
					AllActorCollisionList_.insert(std::map<int, std::list<GH_GameEngineCollision*>>::value_type((*Start)->GH_GetGroupIndex(), std::list<GH_GameEngineCollision*>()));
					//Update/Render의 Order의 값을 key로 가진 각각의 새로운 map을 만든다
				}

				std::map<int, std::list<GH_GameEngineCollision*>>::iterator RenderIter = AllActorCollisionList_.find((*Start)->GH_GetGroupIndex());
				RenderIter->second.push_back((*Start));
			}
		}
	}

	{
		std::map<int, std::list<GH_GameEngineCollision*>>::iterator RenderStart = AllActorCollisionList_.begin();
		std::map<int, std::list<GH_GameEngineCollision*>>::iterator RenderEnd = AllActorCollisionList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GH_GameEngineCollision*>::iterator Start = RenderStart->second.begin();
			std::list<GH_GameEngineCollision*>::iterator End = RenderStart->second.end();
			for (; Start != End; )
			{
				if (RenderStart->first == (*Start)->GH_GetGroupIndex())
				{
					++Start;
					continue;
				}

				Start = RenderStart->second.erase(Start);
			}
		}
	}
}


void GH_Level::GH_Render()
{
	std::map<int, std::list<GH_Actor*>>::iterator RenderStart = AllActorRenderList_.begin();
	std::map<int, std::list<GH_Actor*>>::iterator RenderEnd = AllActorRenderList_.end();

	for (; RenderStart != RenderEnd; ++RenderStart)
	{
		std::list<GH_Actor*>::iterator Start = RenderStart->second.begin();
		std::list<GH_Actor*>::iterator End = RenderStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->GH_Render();
		}
	}

	//순회를 시켜서 모든 Actor들의 Render를 실행시킨다
}

void GH_Level::GH_LevelUpdateBefore()
{
}

void GH_Level::GH_LevelUpdate()
{
}

void GH_Level::GH_LevelUpdateAfter()
{
}

void GH_Level::GH_PushCollision(GH_GameEngineCollision* _collision)
{
	std::map<int, std::list<GH_GameEngineCollision*>>::iterator CollisionIter = AllActorCollisionList_.find(0);
	if (AllActorCollisionList_.end() == CollisionIter)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	CollisionIter->second.push_back(_collision);
	return;
}

void GH_Level::GH_Release()
{
	{
		std::map<int, std::list<GH_GameEngineCollision*>>::iterator RenderStart = AllActorCollisionList_.begin();
		std::map<int, std::list<GH_GameEngineCollision*>>::iterator RenderEnd = AllActorCollisionList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GH_GameEngineCollision*>::iterator Start = RenderStart->second.begin();
			std::list<GH_GameEngineCollision*>::iterator End = RenderStart->second.end();
			for (; Start != End; )
			{
				if (false == (*Start)->GH_IsDeath())
				{
					++Start;
					continue;
				}

				Start = RenderStart->second.erase(Start);
			}
		}
	}

	{
		std::map<int, std::list<GH_Actor*>>::iterator RenderStart = AllActorRenderList_.begin();
		std::map<int, std::list<GH_Actor*>>::iterator RenderEnd = AllActorRenderList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GH_Actor*>::iterator Start = RenderStart->second.begin();
			std::list<GH_Actor*>::iterator End = RenderStart->second.end();
			for (; Start != End; )
			{
				if (true == (*Start)->GH_IsDeath())
				{
					Start = RenderStart->second.erase(Start);	
				}
				else
				{
					++Start;
				}			
			}
		}
	}

	{
		// 지워주는 부분
		// 다른 녀석들을 지워준다.
		std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
		std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();
		for (; UpdateStart != UpdateEnd; ++UpdateStart)
		{
			std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
			std::list<GH_Actor*>::iterator End = UpdateStart->second.end();
			for (; Start != End; )
			{
				if (true == (*Start)->GH_IsDeath())
				{
					delete (*Start);
					Start = UpdateStart->second.erase(Start);
				}
				else
				{
					++Start;
				}
				
			}
		}
	}

}