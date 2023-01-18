#pragma once
#include <GH_Actor.h>
//분류 :
//용도 :
//설명 :
class Normal_Kirby;
class InGame_UI : public GH_Actor
{
	friend Normal_Kirby;
private: //디폴트 접근 지정자
	GH_Renderer* HPBarRender_;
	GH_Renderer* KirbyAbilityRender_;
	GH_Renderer* BossHPBarRender_;
	
public:
	InGame_UI& operator=(const InGame_UI& _Other) = delete; //디폴트 대입연산자
	InGame_UI& operator=(InGame_UI&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	InGame_UI(); //디폴트 생성자
	~InGame_UI(); //디폴트 소멸자
public:
	InGame_UI(const InGame_UI& _Other) = delete; //디폴트 복사 생성자
	InGame_UI(InGame_UI&& _Other) noexcept; //디폴트 RValue 복사생성자
	
public:
	void GH_Update() override;
	//위치정보 TitleSize_를 변화시키는 Update

	void GH_Render() override;
	//위치정보 TitleSize_를 받아서 현재 위치를 기준으로 그림을 그림

	void GH_Start() override;
};

