#pragma once
#include <Windows.h>
#include <string>
#include <GH_GameEngineMath.h>
#include "GH_GameEngineNameBase.h"

class GH_GameEngineImage;
class GH_GameEngineWindow : public GH_GameEngineNameBase
{
	// �з� : ����
	// �뵵 : windowâ�� ����ش�.
	// ���� :
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
	//�� ������â�� ������ȣ

	HWND handle_;
	// �� ������â�� �ڵ�

	//������ â�� ũ��� ��ġ�� ������ �ִ� float4
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
	//������ â�� ũ��� ��ġ�� ���ϴ� �Լ�
	void GH_Loop(void(*_loopFunction)());

private:
	GH_GameEngineWindow();
	//�⺻�����ڸ� private�� ���Ƽ�
	//static�� �̿��� ��쿡�� ����� �հ� ����
	~GH_GameEngineWindow();

};