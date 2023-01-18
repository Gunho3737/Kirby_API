#pragma once
#include <list>
#include <map>
#include <string>
#include <GH_GameEngineDebug.h>
#include <GH_GameEngineNameBase.h>
#include <GH_GameEngineMath.h>


//분류 :
//용도 :
//설명 :
class GH_Actor;
class GH_LevelManager;
class GH_GameEngineCollision;
class GH_Level : public GH_GameEngineNameBase
{
public:
	friend GH_LevelManager;
	friend GH_GameEngineCollision;
	friend GH_Actor;

private: //디폴트 접근 지정자
	bool loadingnow_;
	//현재 로딩상태인지 체크하기 위한 bool
	GH_float4 camPos_;

	std::map<int, std::list<GH_Actor*>> AllActorUpdateList_;
	std::map<int, std::list<GH_Actor*>> AllActorRenderList_;
	//Actor(캐릭터)를 모아둔 list
	//그리고 그 list의 업데이트/렌더링 순서를 정해주기 위한 int을 key값으로 가진 map
	//렌더링 코드 순서를 신경쓰지 않아도 되게 만들기 위한 map이다.

	std::map<int, std::list<GH_GameEngineCollision*>> AllActorCollisionList_;

	std::map<std::string, GH_Actor*> ActorMap_;

private:
	std::list<GH_GameEngineCollision*>& GH_GetCollisionList(int _Group);
	//문자열로 객체를 찾기위해서 map도 만듬

	

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
	GH_Level& operator=(const GH_Level& _Other) = delete; //디폴트 대입연산자
	GH_Level& operator=(GH_Level&& _Other) = delete; //디폴트 RValue 대입연산자
protected:
	GH_Level(); //디폴트 생성자
	~GH_Level(); //디폴트 소멸자
	//자식 클래스들이 이용할수 있게 하기 위해서
public:
	GH_Level(const GH_Level& _Other) = delete; //디폴트 복사 생성자
	GH_Level(GH_Level&& _Other) noexcept; //디폴트 RValue 복사생성자

private:
	void GH_Levelloading()
	{
		if (false == GH_GetloadingNow())
		{
			GH_Loading();
			GH_LoadingEnd();
			//Level 자체의 기능을 넣는곳
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
			//Actor중에 이미 같은 이름인 Actor가 있을 경우, 터트린다
		}

		NewActor->GH_SetName(_actorName);

		//들어온 액터에게 이름과 현재레벨을 세팅해줌

		ActorMap_.insert(std::map<std::string, GH_Actor*>::value_type(_actorName, NewActor));
		//map에 _actorName를 key값으로 가진 Actor를 저장

		return NewActor;
	};


	GH_Actor* GH_FindActor(const std::string& _actorName);

private:
	void GH_PushCollision(GH_GameEngineCollision* _collision);


protected:
	//순수가상함수로 만들어서 이용하지 않을 구현부는 생략
	virtual void GH_Loading() = 0;



	//레벨자체의 로딩
	//ex)지속적인 판단을 필요로 하는경우
	


	virtual void GH_LevelUpdateBefore();
	virtual void GH_LevelUpdate();
	virtual void GH_LevelUpdateAfter();



public:
	//플레이하는 레벨에서 스테이지를 변경하는 함수를 실행하기 위한 가상함수
	virtual void ChangeStage();

	//재시작기능을 넣기 위한 가상함수
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

