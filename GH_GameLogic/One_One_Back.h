#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"

//�з� :
//�뵵 :
//���� :
class Stage_one_one;
class One_One_Back : public GH_Actor
{
	friend Stage_one_one;


public:
	static GH_GameEngineCollision* MapCollision;
	static GH_GameEngineCollision* DoorCollision;

public:
	StageState StageMapState;
	
private:
	DEBUGMODE MapState;
	GH_Renderer* One_One_MainRender_;
	GH_Renderer* One_One_DebugRender_;
	GH_Renderer* One_Two_MainRender_;
	GH_Renderer* One_Two_DebugRender_;
	GH_Renderer* Boss_MainRender_;
	GH_Renderer* Boss_DebugRender_;
	

	GH_float4 One_One_ImageSize_;
	GH_float4 One_Two_ImageSize_;
	GH_float4 Boss_ImageSize_;

public:
	One_One_Back& operator=(const One_One_Back& _Other) = delete; //����Ʈ ���Կ�����
	One_One_Back& operator=(One_One_Back&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	One_One_Back(); //����Ʈ ������
	~One_One_Back(); //����Ʈ �Ҹ���
	One_One_Back(const One_One_Back& _Other) = delete; //����Ʈ ���� ������
	One_One_Back(One_One_Back&& _Other) noexcept; //����Ʈ RValue ���������

public:
	void GH_Update() override;
	//��ġ���� TitleSize_�� ��ȭ��Ű�� Update

	void GH_Render() override;
	//��ġ���� TitleSize_�� �޾Ƽ� ���� ��ġ�� �������� �׸��� �׸�

	void GH_Start() override;


};