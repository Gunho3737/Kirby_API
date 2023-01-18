#pragma once
#include <GH_Actor.h>

//분류 :
//용도 :
//설명 :

class TitleScreen :public GH_Actor
{
private: //디폴트 접근 지정자
	GH_Renderer* MainScreen_;
	GH_Renderer* StartRender_;
	static float TimeCheck_;
	
public:
	TitleScreen& operator=(const TitleScreen& _Other) = delete; //디폴트 대입연산자
	TitleScreen& operator=(TitleScreen&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	TitleScreen(); //디폴트 생성자
	~TitleScreen(); //디폴트 소멸자
	TitleScreen(const TitleScreen& _Other) = delete; //디폴트 복사 생성자
	TitleScreen(TitleScreen&& _Other) noexcept; //디폴트 RValue 복사생성자
	
public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
};

