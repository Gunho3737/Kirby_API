#include "GH_Level.h"
#include "GH_Actor.h"
#include "GH_GameEngineCollision.h"


GH_Level::GH_Level()
	: loadingnow_(false)
	, camPos_(GH_float4::ZERO)
{
	//0�� �׷��� �������
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
	//Stage_One_One�� �Ѱ��ֱ� ���� �����Լ�
}

void GH_Level::StageRestart()
{
	//Stage_One_One�� �Ѱ��ֱ� ���� �����Լ�
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
		//���� ã�� key���� ã�� actor�� ������� ��Ʈ��
	}

	return FindIter->second;
	//ã������ �� key�� value���� ����
}



void GH_Level::GH_UpdateOrderCheck()
{
	{
		//  ���ο� �׷��� �����.
		std::map<int, std::list<GH_Actor*>>::iterator UpdateStart = AllActorUpdateList_.begin();
		std::map<int, std::list<GH_Actor*>>::iterator UpdateEnd = AllActorUpdateList_.end();
		for (; UpdateStart != UpdateEnd; ++UpdateStart)
		{
			std::list<GH_Actor*>::iterator Start = UpdateStart->second.begin();
			std::list<GH_Actor*>::iterator End = UpdateStart->second.end();
			for (; Start != End; ++Start)
			{
				// �̰� �ٸ��ٴ°���
				// �̳༮�� �ڽ��� ������ �ٲٰڴٴ� �̾߱Ⱑ �ȴ�.
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
		// �����ִ� �κ�
		// �ٸ� �༮���� �����ش�.
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

	//��ȸ�� ���Ѽ� ��� Actor���� UpdateBefore�� �����Ų��

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

	//��ȸ�� ���Ѽ� ��� Actor���� Update�� �����Ų��
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

	//��ȸ�� ���Ѽ� ��� Actor���� UpdateAfter�� �����Ų��
}

void GH_Level::GH_RenderOrderCheck()
{
	{
		//���ο� �׷��� �����
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
					//Update/Render�� Order�� ���� key�� ���� ������ ���ο� map�� �����
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
		//���ο� �׷��� �����
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
					//Update/Render�� Order�� ���� key�� ���� ������ ���ο� map�� �����
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

	//��ȸ�� ���Ѽ� ��� Actor���� Render�� �����Ų��
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
		// �����ִ� �κ�
		// �ٸ� �༮���� �����ش�.
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