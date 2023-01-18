#pragma once
#include "GameEngineEnum.h"
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
#include <GH_GameEngineDebug.h>
#include <list>
#include <windows.h>

//�з� :
//�뵵 :
//���� :
class GH_Actor;
class GH_Level;
class GH_LevelManager;
class GH_GameEngineImage;
class GH_GameEngineCollision : public GH_GameEngineNameBase
{
private:
	static bool (*GH_arrFunc_[static_cast<int>(GH_CollisionCheckType::MAX)][static_cast<int>(GH_CollisionCheckType::MAX)])(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol);
	//�Լ� �������� ������ �迭
	//�Լ����� arrFunc_
	//[][] ����(sizeof(arrFunc_)��) max������ �������� �޶���
	//����()���ںκ�=> �̰ɷ� ���ؼ� �ҷ����� �Լ����� ����
	//�Լ� ������ arrFunc_[0][1]�� �� �ִ� �Լ��� ���ڴ�
	//(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol)�� �Ǿ� �ϴ� ���̴�.
	// 
	//*�Լ� ������=> �ڷ��� (*�Լ���)(������ �ȿ� �� �Լ��� ������ �ڷ���)�̹Ƿ�
	static void GH_ColCheckFuncInitialize();

public:
	//�̰��� �浹�� ��� ����Ǽ��� �־���Ѵ�
	static bool GH_PointToImage(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol);
	static bool GH_ImageToPoint(GH_GameEngineCollision* _ImageCol, GH_GameEngineCollision* _PointCol);
	static bool GH_RectToRect(GH_GameEngineCollision* _Left, GH_GameEngineCollision* _Right);
private:
	friend GH_LevelManager;
	friend GH_Actor;
	friend GH_Level;

private: //����Ʈ ���� ������
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
		//dynamic_cast => �����Լ��� ����ϰ� �ִ� �θ��ڽİ� ���迡����
		// ������ ����ȯ�� �����ϰ� ���ִ� �������̴�

		ActorType* convertptr = nullptr;
		if (nullptr != Actor_)
		{
			convertptr = dynamic_cast<ActorType*>(Actor_);
		}

		if (nullptr == convertptr)
		{
			GH_GameEngineDebug::MessageBoxError("actor ����Ʈ ����");
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
		//���� �� �浹�� �Ͼ�� ���͵� �޾ƿ� �� �ִ�
		Actor_ = _parent;
	}
	
public:
	GH_GameEngineCollision& operator=(const GH_GameEngineCollision& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineCollision& operator=(GH_GameEngineCollision&& _Other) = delete; //����Ʈ RValue ���Կ�����
private:
	GH_GameEngineCollision(); //����Ʈ ������
	~GH_GameEngineCollision(); //����Ʈ �Ҹ���
public:
	GH_GameEngineCollision(const GH_GameEngineCollision& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineCollision(GH_GameEngineCollision&& _Other) noexcept; //����Ʈ RValue ���������
	
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

