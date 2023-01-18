#pragma once
#include "GameEngineEnum.h"
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
#include <GH_GameEngineDebug.h>
#include <list>
#include <windows.h>

//분류 :
//용도 :
//설명 :
class GH_Actor;
class GH_Level;
class GH_LevelManager;
class GH_GameEngineImage;
class GH_GameEngineCollision : public GH_GameEngineNameBase
{
private:
	static bool (*GH_arrFunc_[static_cast<int>(GH_CollisionCheckType::MAX)][static_cast<int>(GH_CollisionCheckType::MAX)])(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol);
	//함수 포인터의 이차원 배열
	//함수명은 arrFunc_
	//[][] 갯수(sizeof(arrFunc_)는) max까지의 값에따라 달라짐
	//뒤의()인자부분=> 이걸로 인해서 불러와질 함수들의 인자
	//함수 포인터 arrFunc_[0][1]에 들어가 있는 함수의 인자는
	//(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol)가 되야 하는 것이다.
	// 
	//*함수 포인터=> 자료형 (*함수명)(포인터 안에 들어갈 함수의 인자의 자료형)이므로
	static void GH_ColCheckFuncInitialize();

public:
	//이곳에 충돌의 모든 경우의수를 넣어야한다
	static bool GH_PointToImage(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol);
	static bool GH_ImageToPoint(GH_GameEngineCollision* _ImageCol, GH_GameEngineCollision* _PointCol);
	static bool GH_RectToRect(GH_GameEngineCollision* _Left, GH_GameEngineCollision* _Right);
private:
	friend GH_LevelManager;
	friend GH_Actor;
	friend GH_Level;

private: //디폴트 접근 지정자
	GH_Actor* Actor_;
	GH_float4 Pivot_;
	GH_float4 Size_;

	GH_CollisionCheckType collisionType_;
	int GroupIndex_;

	GH_GameEngineImage* ImagePtr_;
	DWORD CheckColor_;

private:
	GH_float4 GH_GetCollisionPos();

public:
	GH_Actor* GH_GetActor()
	{
		return Actor_;
	}

	template<typename ActorType>
	ActorType* GH_GetActorConvert()
	{
		//dynamic_cast => 가상함수를 사용하고 있는 부모자식간 관계에서만
		// 안전한 형변환을 가능하게 해주는 연산자이다

		ActorType* convertptr = nullptr;
		if (nullptr != Actor_)
		{
			convertptr = dynamic_cast<ActorType*>(Actor_);
		}

		if (nullptr == convertptr)
		{
			GH_GameEngineDebug::MessageBoxError("actor 컨버트 에러");
		}

		return convertptr;
	}

public:
	void GH_SetImage(std::string _ImageName);

public:
	void GH_SetColorCheck(DWORD _Color)
	{
		CheckColor_ = _Color;
	}

	void GH_SetGroupIndex(int _Index)
	{
		GroupIndex_ = _Index;
	}

	void GH_SetColType(GH_CollisionCheckType _Type)
	{
		collisionType_ = _Type;
	}

	void GH_SetPivot(GH_float4 _pivot)
	{
		Pivot_ = _pivot;
	}

	void GH_SetSize(GH_float4 _size)
	{
		Size_ = _size;
	}
	
public:
	int GH_GetTypeToIndex()
	{
		return static_cast<int>(collisionType_);
	}

	int GH_GetGroupIndex()
	{
		return GroupIndex_;
	}

	DWORD GH_GetColorCheck()
	{
		return CheckColor_;
	}

	bool isCamEffect_;

private:
	void GH_SetActor(GH_Actor* _parent)
	{
		//지금 이 충돌이 일어나는 액터도 받아올 수 있다
		Actor_ = _parent;
	}
	
public:
	GH_GameEngineCollision& operator=(const GH_GameEngineCollision& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineCollision& operator=(GH_GameEngineCollision&& _Other) = delete; //디폴트 RValue 대입연산자
private:
	GH_GameEngineCollision(); //디폴트 생성자
	~GH_GameEngineCollision(); //디폴트 소멸자
public:
	GH_GameEngineCollision(const GH_GameEngineCollision& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineCollision(GH_GameEngineCollision&& _Other) noexcept; //디폴트 RValue 복사생성자
	
public:
	void GH_DebugRender();

	bool GH_CollisionCheck(GH_GameEngineCollision* _other);

	template<typename COLLIGIONGROUP>
	GH_GameEngineCollision* GH_CollisionGroupCheckOne(COLLIGIONGROUP _Othergroup)
	{
		return GH_CollisionGroupCheckOne(static_cast<int>(_Othergroup));
	}

	GH_GameEngineCollision* GH_CollisionGroupCheckOne(int _otherIndex);
	//std::list<GH_GameEngineCollision*> GH_ColligionGroupCheck(int _otherIndex);

public:
	GH_Figure GH_GetFigure();

};

