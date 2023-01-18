#pragma once
#include <GH_Actor.h>

//분류 :
//용도 :
//설명 :

class EndScreen :public GH_Actor
{
private: //디폴트 접근 지정자
	GH_Renderer* MainScreen_;



public:
	EndScreen& operator=(const EndScreen& _Other) = delete; //디폴트 대입연산자
	EndScreen& operator=(EndScreen&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	EndScreen(); //디폴트 생성자
	~EndScreen(); //디폴트 소멸자
	EndScreen(const EndScreen& _Other) = delete; //디폴트 복사 생성자
	EndScreen(EndScreen&& _Other) noexcept; //디폴트 RValue 복사생성자

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
};


