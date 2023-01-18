#pragma once
#include <GH_GameEngineNameBase.h>
#include <map>
#include <string>

//분류 :
//용도 :
//설명 :
class GH_GameEngineWindow;
class GH_GameEngineInput
{
private:
	friend GH_GameEngineWindow;

private: //디폴트 접근 지정자
	class GH_GameEngineKey : public GH_GameEngineNameBase
	{
		friend GH_GameEngineInput;

	private:

		bool down_; //키가 눌렷을 때 한번
		bool press_; //키가 지속적으로 눌리고 있을때
		bool up_; //키를 누르지 않고 뗏을때 한번
		bool free_; //키가 눌리지 않은 상태
		int key_; //내가 누르려는 키의 인덱스 혹은 타입

	public:
		void GH_Update();

	private:
		GH_GameEngineKey();
		~GH_GameEngineKey();
	};

public:
	static GH_GameEngineInput* Instance;

	static GH_GameEngineInput& GH_GetInst()
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
	std::map<std::string, GH_GameEngineKey*> allkeys_;



public:
	GH_GameEngineInput& operator=(const GH_GameEngineInput& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineInput& operator=(GH_GameEngineInput&& _Other) = delete; //디폴트 RValue 대입연산자
private:
	GH_GameEngineInput(); //디폴트 생성자
	~GH_GameEngineInput(); //디폴트 소멸자
public:
	GH_GameEngineInput(const GH_GameEngineInput& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineInput(GH_GameEngineInput&& _Other) noexcept; //디폴트 RValue 복사생성자


public:
	//윈도우에서 특정키를 사용가능하게 만드는 함수
	bool GH_CreateKey(const std::string _name, int _key);

	//지금 특정키가 특수한 상태에 있는가? 를 bool로 체크하는 함수
	bool GH_IsDown(const std::string _name); //키가 눌렷는가
	bool GH_IsUp(const std::string _name); //키가 떼어졌는가
	bool GH_IsPress(const std::string _name); //키가 눌리고 있는가
	bool GH_IsFree(const std::string _name); //키가 떼어지고 있는 상태인가

	//현재 무슨 키가 만들어 졌는가?를 체크하는 함수
	bool GH_IsKey(const std::string _name);

private:
	GH_GameEngineKey* GH_FindKey(const std::string _name);
	void GH_Update();
};

