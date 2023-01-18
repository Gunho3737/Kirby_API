#pragma once
#include <map>
#include <string>
#include <list>
#include <Windows.h>

//분류 :
//용도 :
//설명 :
class GH_LevelManager;
class GH_GameEngineImage;
class GH_GameEngineWindow;
class GH_ResourcesManager
{
public:
	friend GH_GameEngineWindow;
	friend GH_LevelManager;

private: //디폴트 접근 지정자
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
	GH_ResourcesManager& operator=(const GH_ResourcesManager& _Other) = delete; //디폴트 대입연산자
	GH_ResourcesManager& operator=(GH_ResourcesManager&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_ResourcesManager(); //디폴트 생성자
	~GH_ResourcesManager(); //디폴트 소멸자
	GH_ResourcesManager(const GH_ResourcesManager& _Other) = delete; //디폴트 복사 생성자
	GH_ResourcesManager(GH_ResourcesManager&& _Other) noexcept; //디폴트 RValue 복사생성자

};

