#pragma once
#include "GH_GameEngineSound.h"
#include "GH_GameEngineSoundFile.h"
//분류 :
//용도 :
//설명 :

class GH_GameEngineSoundPlayer
{
	friend GH_GameEngineSound;

private: //디폴트 접근 지정자
	GH_GameEngineSound* PlaySoundFile_;

	FMOD::Channel* PlayChannel_;
	//사운드의 재생/정지를 담당하는 FMOD 인터페이스

	int PlayCount;
	//소리 반복횟수를 결정
	//사용후에는 PlayCountReset으로 횟수를 다시 초기화 시켜줘야한다
	
public:
	GH_GameEngineSoundPlayer& operator=(const GH_GameEngineSoundPlayer& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineSoundPlayer& operator=(GH_GameEngineSoundPlayer&& _Other) = delete; //디폴트 RValue 대입연산자
private:
	GH_GameEngineSoundPlayer(); //디폴트 생성자
	~GH_GameEngineSoundPlayer(); //디폴트 소멸자
public:
	GH_GameEngineSoundPlayer(const GH_GameEngineSoundPlayer& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineSoundPlayer(GH_GameEngineSoundPlayer&& _Other) noexcept; //디폴트 RValue 복사생성자
	
public:
	bool GH_IsPlay();


	void GH_PlayCountReset(int _count = 1);

	//사운드를 겹쳐서 사용할경우 사용하는 함수
	void GH_PlayOverLap(const std::string& _name, int LoopCount = 0);

	//사운드를 겹쳐서 실행하지 못하게 하고 무조건 하나만 실행될수 있는 사운드 재생함수
	void GH_PlayAlone(const std::string& _name, int LoopCount = 0);


	void GH_Stop();
};

