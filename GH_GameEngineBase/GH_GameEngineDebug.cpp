#include "GH_GameEngineDebug.h"
#include <assert.h>
#include <crtdbg.h>
#include <Windows.h>

GH_GameEngineDebug::GH_GameEngineDebug()
{

}

GH_GameEngineDebug::~GH_GameEngineDebug()
{
}

GH_GameEngineDebug::GH_GameEngineDebug(GH_GameEngineDebug&& _Other) noexcept
{
}

void GH_GameEngineDebug::MessageBoxError(const char* _Ptr)
{
	MessageBoxA(nullptr, _Ptr, "Error", MB_OK);
	assert(false);
}

void GH_GameEngineDebug::Assert()
{
	assert(false);
}

void GH_GameEngineDebug::GH_LeakCheckOn()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
}