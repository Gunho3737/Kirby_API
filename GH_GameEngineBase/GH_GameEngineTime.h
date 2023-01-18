#pragma once
#include <Windows.h>

//분류 :
//용도 :
//설명 :

class GH_GameEngineTime
{

private:
	static GH_GameEngineTime* Instance;

public:
	static GH_GameEngineTime& GH_GetInst()
	{
		return *Instance;
	}

	static void GH_Destory()
	{
		if (nullptr != Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	LARGE_INTEGER timeCount_;
	LARGE_INTEGER startCheck_;
	LARGE_INTEGER endCheck_;
	double deltaTime_;

public:
	// 여기에 이렇게 헤더에 구현한 이유
	// 리턴하는게 기본자료형이어서
	double GH_GetDeltaTimeD()
	{
		return deltaTime_;
	}

	float GH_GetDeltaTime()
	{
		return static_cast<float>(deltaTime_);
	}

public:
	GH_GameEngineTime(); // default constructer 디폴트 생성자
	~GH_GameEngineTime(); // default destructer 디폴트 소멸자

public:		// delete constructer
	GH_GameEngineTime(const GH_GameEngineTime& _Other) = delete; // default Copy constructer 디폴트 복사생성자
	GH_GameEngineTime(const GH_GameEngineTime&& _Other); // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	GH_GameEngineTime& operator=(const GH_GameEngineTime& _Other) = delete; // default Copy operator 디폴트 대입 연산자
	GH_GameEngineTime& operator=(const GH_GameEngineTime&& _Other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void GH_TimeCheckReset();
	void GH_TimeCheck();
};

