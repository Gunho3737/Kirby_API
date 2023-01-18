#pragma once
#include <GH_Level.h>

//분류 :
//용도 :
//설명 :
class GH_GameEngineSoundPlayer;
class EndLevel : public GH_Level
{
private: //디폴트 접근 지정자
	GH_GameEngineSoundPlayer* BackgroundPlayer;
	
	
	
public:
	EndLevel& operator=(const EndLevel& _Other) = delete; //디폴트 대입연산자
	EndLevel& operator=(EndLevel&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	EndLevel(); //디폴트 생성자
	~EndLevel(); //디폴트 소멸자
	EndLevel(const EndLevel& _Other) = delete; //디폴트 복사 생성자
	EndLevel(EndLevel&& _Other) noexcept; //디폴트 RValue 복사생성자

public:
	void GH_Loading() override;
	//부모 클래스인 Level의 Loading대신 실행

	void GH_LevelUpdate() override;
	
};

