#pragma once
#include <map>
#include <string>
#include <list>
#include <Windows.h>

//�з� :
//�뵵 :
//���� :
class GH_LevelManager;
class GH_GameEngineImage;
class GH_GameEngineWindow;
class GH_ResourcesManager
{
public:
	friend GH_GameEngineWindow;
	friend GH_LevelManager;

private: //����Ʈ ���� ������
	static GH_ResourcesManager* Instance;

public:

	static GH_ResourcesManager& GH_GetInst()
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
	GH_GameEngineImage* WindowImage_;
	GH_GameEngineImage* BackBufferImage_;

private:
	void GH_InitializeWindowImage(HDC _windowbackgroundDC);
	void DoubleBuffering();

public:
	GH_GameEngineImage* GH_GetBackBufferImage();

private:
	std::map<std::string, GH_GameEngineImage*> GlobalManagedImages_;


public:
	GH_GameEngineImage* GH_LoadGameImage(std::string _name, std::string _path);

	GH_GameEngineImage* GH_FindGameImage(std::string _name);


public:
	GH_ResourcesManager& operator=(const GH_ResourcesManager& _Other) = delete; //����Ʈ ���Կ�����
	GH_ResourcesManager& operator=(GH_ResourcesManager&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_ResourcesManager(); //����Ʈ ������
	~GH_ResourcesManager(); //����Ʈ �Ҹ���
	GH_ResourcesManager(const GH_ResourcesManager& _Other) = delete; //����Ʈ ���� ������
	GH_ResourcesManager(GH_ResourcesManager&& _Other) noexcept; //����Ʈ RValue ���������

};

