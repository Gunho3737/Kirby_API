#pragma once
#include <map>
#include <string>
#include <GH_GameEngineDebug.h>
#include "GH_Level.h"

//분류 : 매니저(관리클래스)
//용도 : 레벨관리
//설명 : 레벨을만들고
//       레벨을 전환시켜줄 수 있는 기능을 담당
//class GH_Level;
class GH_LevelManager
{
private:
	static GH_LevelManager* Instance;

private:
	std::map<std::string, GH_Level*> LevelMap_;
	//문자열 key로 사용해 Level 자료들을 관리하는 map

	GH_Level* CurrentLevel_;
	//현재 내가 무슨 레벨(장면)에 있는지를 저장하는 변수

	GH_Level* NextLevel_;
	//다음 레벨을 저장하는 변수

private:
	GH_LevelManager(); //디폴트 생성자
	~GH_LevelManager(); //디폴트 소멸자

public:
	GH_LevelManager& operator=(const GH_LevelManager& _Other) = delete; //디폴트 대입연산자
	GH_LevelManager& operator=(GH_LevelManager&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_LevelManager(const GH_LevelManager& _Other) = delete; //디폴트 복사 생성자
	GH_LevelManager(GH_LevelManager&& _Other) noexcept; //디폴트 RValue 복사생성자
public:
	static GH_LevelManager& GH_GetInst()
	{
		return *Instance;
		//static 이므로 .h에 선언까지 했다
		//static을 이용한 싱글톤
	}

	static void GH_Destory()
	{
		if (nullptr != Instance)
		{
			delete Instance;
			Instance = nullptr;
			//동적 할당 했으므로 힙이 안남게 delete 해줘야함

		}
	}

public:
	template<typename LevelType>
	void GH_CreateLevel(std::string _LVname)
	{
		if (LevelMap_.end() != LevelMap_.find(_LVname))
		{
			GH_GameEngineDebug::Assert();
			return;
			//이미 만들어진 이름의 레벨이면 터트린다
		}

		GH_Level* newLevel = new LevelType();
		//자식 클래스의 자료형에 맞춰서 동적할당
		newLevel->GH_Levelloading();
		//현재 레벨에 필요한 actor등을 가져옴
		//virtual을 이용해서 자식의 함수를 실행시킴

		newLevel->GH_SetName(_LVname);
		//문제가 생겼을때, 고치기 편하게 하기 위해 이름을 붙혀줬다

		LevelMap_.insert(std::map<std::string, GH_Level*>::value_type(_LVname, newLevel));
		//레벨을 map에 저장한다
	}


	void GH_ChangeLevel(std::string _LVname);

	void GH_Update();
};

