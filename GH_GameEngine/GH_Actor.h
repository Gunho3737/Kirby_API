#pragma once
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
#include <GH_GameEngineDebug.h>
#include <list>
#include "GameEngineEnum.h"

//분류 :
//용도 :
//설명 :
class GH_Level;
class GH_Renderer;
class GH_GameEngineCollision;
class GH_Actor : public GH_GameEngineNameBase
{
private: //디폴트 접근 지정자
	friend GH_Level;

private:
	GH_Level* ParentLevel_;
	int UpdateOrder_;
	int RenderOrder_;

public:
	void GH_SetLevel(GH_Level* _ParentLevel)
	{
		ParentLevel_ = _ParentLevel;
	}

	GH_Level* GH_GetLevel()
	{
		if (nullptr == ParentLevel_)
		{
			GH_GameEngineDebug::Assert();
			return nullptr;
		}

		return ParentLevel_;
	}

	void GH_SetUpdateOrder(int _UpdateOrder)
	{
		UpdateOrder_ = _UpdateOrder;
	}

	void GH_SetRenderOrder(int _RenderOrder)
	{
		RenderOrder_ = _RenderOrder;
	}
private:
	int ActorNumber;
	//Actor 마다 고유의 Number을 준다

	GH_float4 pos_;
	//Actor의 현재 위치정보를 가지고 있다
public:
	GH_float4 GH_GetPos()
	{
		return pos_;
		//Actor의 현재 위치를 리턴해주는 함수
	}

	GH_float4 GH_GetCamEffectPos();

public:
	void GH_SetPos(GH_float4 _pos)
	{
		pos_ = _pos;
		//위치를 정하는 함수
	}

	void GH_SetMove(GH_float4 _pos)
	{
		pos_ += _pos;
		//위치를 바꾸는 함수
		//LEFT/RIGHT/UP/DOWN 을 더해서 pos를 바꿈
	}

protected:
	GH_Actor(); //디폴트 생성자
	~GH_Actor(); //디폴트 소멸자


public:
	GH_Actor& operator=(const GH_Actor& _Other) = delete; //디폴트 대입연산자
	GH_Actor& operator=(GH_Actor&& _Other) = delete; //디폴트 RValue 대입연산자
public:

	GH_Actor(const GH_Actor& _Other) = delete; //디폴트 복사 생성자
	GH_Actor(GH_Actor&& _Other) noexcept; //디폴트 RValue 복사생성자

private:
	//만들어지는 순간에 한번 실행되는 함수
	virtual void GH_Start();

	//반복되는 함수
	virtual void GH_UpdateBefore();
	virtual void GH_Update();
	virtual void GH_UpdateAfter();
	virtual void GH_Render();
	virtual void GH_Collision();

private:
	std::list<GH_Renderer*> RendererList_;

public:
	GH_Renderer* GH_CreateRenderer(std::string _ImageName);

private:
	std::list<GH_GameEngineCollision*> CollisionList_;
	//모든 충돌들을 리스트로 저장
	//그룹과 충돌타입이 저장된다

public:
	template<typename T>
	GH_GameEngineCollision* GH_CreateCollision(T _group, GH_CollisionCheckType _type)
	{
		return GH_CreateCollision(static_cast<int>(_group), _type);
	}

	GH_GameEngineCollision* GH_CreateCollision(int _Group, GH_CollisionCheckType _type);
	//GameLogicEnum의 그룹에 맞춰서 내가 그룹인지(map이나 player)와, 무슨 충돌타입인가를 받는것
	//단, 인자는 int인데 실제로 들어오는건 enum이라 static_cast(형변환)을 할 필요가 있음
};

