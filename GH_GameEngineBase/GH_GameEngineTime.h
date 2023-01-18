#pragma once
#include <Windows.h>

//�з� :
//�뵵 :
//���� :

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
	// ���⿡ �̷��� ����� ������ ����
	// �����ϴ°� �⺻�ڷ����̾
	double GH_GetDeltaTimeD()
	{
		return deltaTime_;
	}

	float GH_GetDeltaTime()
	{
		return static_cast<float>(deltaTime_);
	}

public:
	GH_GameEngineTime(); // default constructer ����Ʈ ������
	~GH_GameEngineTime(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	GH_GameEngineTime(const GH_GameEngineTime& _Other) = delete; // default Copy constructer ����Ʈ ���������
	GH_GameEngineTime(const GH_GameEngineTime&& _Other); // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	GH_GameEngineTime& operator=(const GH_GameEngineTime& _Other) = delete; // default Copy operator ����Ʈ ���� ������
	GH_GameEngineTime& operator=(const GH_GameEngineTime&& _Other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void GH_TimeCheckReset();
	void GH_TimeCheck();
};

