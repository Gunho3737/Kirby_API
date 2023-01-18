#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"

//분류 :
//용도 :
//설명 :
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
	One_One_Back& operator=(const One_One_Back& _Other) = delete; //디폴트 대입연산자
	One_One_Back& operator=(One_One_Back&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	One_One_Back(); //디폴트 생성자
	~One_One_Back(); //디폴트 소멸자
	One_One_Back(const One_One_Back& _Other) = delete; //디폴트 복사 생성자
	One_One_Back(One_One_Back&& _Other) noexcept; //디폴트 RValue 복사생성자

public:
	void GH_Update() override;
	//위치정보 TitleSize_를 변화시키는 Update

	void GH_Render() override;
	//위치정보 TitleSize_를 받아서 현재 위치를 기준으로 그림을 그림

	void GH_Start() override;


};