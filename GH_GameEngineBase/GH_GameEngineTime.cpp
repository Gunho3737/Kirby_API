#include "GH_GameEngineTime.h"

GH_GameEngineTime* GH_GameEngineTime::Instance = new GH_GameEngineTime();
// Static Func
// constructer destructer
GH_GameEngineTime::GH_GameEngineTime()
{
	GH_TimeCheckReset();
}

GH_GameEngineTime::~GH_GameEngineTime()
{
}

GH_GameEngineTime::GH_GameEngineTime(const GH_GameEngineTime&& _Other)
	: timeCount_(_Other.timeCount_),
	startCheck_(_Other.startCheck_),
	endCheck_(_Other.endCheck_)
{
}

// �ð��� �غ� �Ѵ�.
void GH_GameEngineTime::GH_TimeCheckReset()
{
	// window�� ���� �������ϴ�.
	QueryPerformanceFrequency(&timeCount_);
	QueryPerformanceCounter(&startCheck_);
	QueryPerformanceCounter(&endCheck_);
}

void GH_GameEngineTime::GH_TimeCheck()
{
	QueryPerformanceCounter(&endCheck_);
	deltaTime_ = static_cast<double>((endCheck_.QuadPart - startCheck_.QuadPart)) / static_cast<double>(timeCount_.QuadPart);
	startCheck_.QuadPart = endCheck_.QuadPart;
}