#pragma once
#include <Windows.h>
#include <string>
#include <GH_GameEngineMath.h>
#include "GH_GameEngineNameBase.h"

class GH_GameEngineImage;
class GH_GameEngineWindow : public GH_GameEngineNameBase
{
	// 분류 : 엔진
	// 용도 : window창을 띄워준다.
	// 설명 :
private:
	static GH_GameEngineWindow* Instance;

public:
	static GH_GameEngineWindow& GH_GetInst()
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
	std::string ClassName_;
	std::string WindowTitle_;

	HINSTANCE HInstance_;
	//내 윈도우창의 고유번호

	HWND handle_;
	// 내 윈도우창의 핸들

	//윈도우 창의 크기와 위치를 가지고 있는 float4
	GH_float4 size_;
	GH_float4 pos_;

public:
	GH_float4 GH_GetSize()
	{
		return size_;
	}

	GH_float4 GH_GetPos()
	{
		return pos_;
	}
public:
	void GH_CreateWindowClass(HINSTANCE _hInstance, std::string _className);
	void GH_CreateMainWindow(std::string _titlename, const GH_float4& _size, const GH_float4& _pos);
	void GH_SetSizeAndPos(const GH_float4& _size, const GH_float4& _pos);
	//윈도우 창의 크기와 위치를 정하는 함수
	void GH_Loop(void(*_loopFunction)());

private:
	GH_GameEngineWindow();
	//기본생성자를 private로 막아서
	//static을 이용한 경우에만 만들수 잇게 해줌
	~GH_GameEngineWindow();

};