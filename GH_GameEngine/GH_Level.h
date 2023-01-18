#pragma once
#include <list>
#include <map>
#include <string>
#include <GH_GameEngineDebug.h>
#include <GH_GameEngineNameBase.h>
#include <GH_GameEngineMath.h>


//�з� :
//�뵵 :
//���� :
class GH_Actor;
class GH_LevelManager;
class GH_GameEngineCollision;
class GH_Level : public GH_GameEngineNameBase
{
public:
	friend GH_LevelManager;
	friend GH_GameEngineCollision;
	friend GH_Actor;

private: //����Ʈ ���� ������
	bool loadingnow_;
	//���� �ε��������� üũ�ϱ� ���� bool
	GH_float4 camPos_;

	std::map<int, std::list<GH_Actor*>> AllActorUpdateList_;
	std::map<int, std::list<GH_Actor*>> AllActorRenderList_;
	//Actor(ĳ����)�� ��Ƶ� list
	//�׸��� �� list�� ������Ʈ/������ ������ �����ֱ� ���� int�� key������ ���� map
	//������ �ڵ� ������ �Ű澲�� �ʾƵ� �ǰ� ����� ���� map�̴�.

	std::map<int, std::list<GH_GameEngineCollision*>> AllActorCollisionList_;

	std::map<std::string, GH_Actor*> ActorMap_;

private:
	std::list<GH_GameEngineCollision*>& GH_GetCollisionList(int _Group);
	//���ڿ��� ��ü�� ã�����ؼ� map�� ����

	

public:
	GH_float4 GH_GetCamPos()
	{
		return camPos_;
	}

	void GH_SetCamMove(GH_float4 _pos)
	{
		camPos_ += _pos;
	}

	void GH_SetCamPos(GH_float4 _pos)
	{
		camPos_ = _pos;
	}
public:
	GH_Level& operator=(const GH_Level& _Other) = delete; //����Ʈ ���Կ�����
	GH_Level& operator=(GH_Level&& _Other) = delete; //����Ʈ RValue ���Կ�����
protected:
	GH_Level(); //����Ʈ ������
	~GH_Level(); //����Ʈ �Ҹ���
	//�ڽ� Ŭ�������� �̿��Ҽ� �ְ� �ϱ� ���ؼ�
public:
	GH_Level(const GH_Level& _Other) = delete; //����Ʈ ���� ������
	GH_Level(GH_Level&& _Other) noexcept; //����Ʈ RValue ���������

private:
	void GH_Levelloading()
	{
		if (false == GH_GetloadingNow())
		{
			GH_Loading();
			GH_LoadingEnd();
			//Level ��ü�� ����� �ִ°�
		}
	}

	bool GH_GetloadingNow()
	{
		return loadingnow_;
	}

	void GH_LoadingEnd()
	{
		loadingnow_ = true;
	}


public:
	template<typename ActorType>
	ActorType* GH_CreateActor()
	{
		std::map<int, std::list<GH_Actor*>>::iterator UpdateIter = AllActorUpdateList_.find(0);
		if (AllActorUpdateList_.end() == UpdateIter)
		{
			GH_GameEngineDebug::Assert();
			return nullptr;
		}

		std::map<int, std::list<GH_Actor*>>::iterator RenderIter = AllActorRenderList_.find(0);
		if (AllActorRenderList_.end() == RenderIter)
		{
			GH_GameEngineDebug::Assert();
			return nullptr;
		}

		ActorType* NewActor = new ActorType();
		NewActor->GH_SetName("Actor");
		NewActor->GH_SetLevel(this);

		NewActor->GH_Start();


		UpdateIter->second.push_back(NewActor);
		RenderIter->second.push_back(NewActor);
		return NewActor;

	};

	template<typename ActorType>
	ActorType* GH_CreateActor(const std::string& _actorName)
	{
		ActorType* NewActor = GH_CreateActor<ActorType>();

		if (ActorMap_.end() != ActorMap_.find(_actorName))
		{
			GH_GameEngineDebug::Assert();
			return nullptr;
			//Actor�߿� �̹� ���� �̸��� Actor�� ���� ���, ��Ʈ����
		}

		NewActor->GH_SetName(_actorName);

		//���� ���Ϳ��� �̸��� ���緹���� ��������

		ActorMap_.insert(std::map<std::string, GH_Actor*>::value_type(_actorName, NewActor));
		//map�� _actorName�� key������ ���� Actor�� ����

		return NewActor;
	};


	GH_Actor* GH_FindActor(const std::string& _actorName);

private:
	void GH_PushCollision(GH_GameEngineCollision* _collision);


protected:
	//���������Լ��� ���� �̿����� ���� �����δ� ����
	virtual void GH_Loading() = 0;



	//������ü�� �ε�
	//ex)�������� �Ǵ��� �ʿ�� �ϴ°��
	


	virtual void GH_LevelUpdateBefore();
	virtual void GH_LevelUpdate();
	virtual void GH_LevelUpdateAfter();



public:
	//�÷����ϴ� �������� ���������� �����ϴ� �Լ��� �����ϱ� ���� �����Լ�
	virtual void ChangeStage();

	//����۱���� �ֱ� ���� �����Լ�
	virtual void StageRestart();
	
public:
	void GH_UpdateOrderCheck();
	void GH_CollisionOrderCheck();
	void GH_RenderOrderCheck();

	void GH_UpdateBefore();
	void GH_Update();
	void GH_UpdateAfter();

	void GH_Collision();
	void GH_Render();
	void GH_Release();
};

