#pragma once
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
#include <GH_GameEngineDebug.h>
#include <list>
#include "GameEngineEnum.h"

//�з� :
//�뵵 :
//���� :
class GH_Level;
class GH_Renderer;
class GH_GameEngineCollision;
class GH_Actor : public GH_GameEngineNameBase
{
private: //����Ʈ ���� ������
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
	//Actor ���� ������ Number�� �ش�

	GH_float4 pos_;
	//Actor�� ���� ��ġ������ ������ �ִ�
public:
	GH_float4 GH_GetPos()
	{
		return pos_;
		//Actor�� ���� ��ġ�� �������ִ� �Լ�
	}

	GH_float4 GH_GetCamEffectPos();

public:
	void GH_SetPos(GH_float4 _pos)
	{
		pos_ = _pos;
		//��ġ�� ���ϴ� �Լ�
	}

	void GH_SetMove(GH_float4 _pos)
	{
		pos_ += _pos;
		//��ġ�� �ٲٴ� �Լ�
		//LEFT/RIGHT/UP/DOWN �� ���ؼ� pos�� �ٲ�
	}

protected:
	GH_Actor(); //����Ʈ ������
	~GH_Actor(); //����Ʈ �Ҹ���


public:
	GH_Actor& operator=(const GH_Actor& _Other) = delete; //����Ʈ ���Կ�����
	GH_Actor& operator=(GH_Actor&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:

	GH_Actor(const GH_Actor& _Other) = delete; //����Ʈ ���� ������
	GH_Actor(GH_Actor&& _Other) noexcept; //����Ʈ RValue ���������

private:
	//��������� ������ �ѹ� ����Ǵ� �Լ�
	virtual void GH_Start();

	//�ݺ��Ǵ� �Լ�
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
	//��� �浹���� ����Ʈ�� ����
	//�׷�� �浹Ÿ���� ����ȴ�

public:
	template<typename T>
	GH_GameEngineCollision* GH_CreateCollision(T _group, GH_CollisionCheckType _type)
	{
		return GH_CreateCollision(static_cast<int>(_group), _type);
	}

	GH_GameEngineCollision* GH_CreateCollision(int _Group, GH_CollisionCheckType _type);
	//GameLogicEnum�� �׷쿡 ���缭 ���� �׷�����(map�̳� player)��, ���� �浹Ÿ���ΰ��� �޴°�
	//��, ���ڴ� int�ε� ������ �����°� enum�̶� static_cast(����ȯ)�� �� �ʿ䰡 ����
};

