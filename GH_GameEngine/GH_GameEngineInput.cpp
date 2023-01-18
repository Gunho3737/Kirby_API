#include "GH_GameEngineInput.h"
#include <Windows.h>
#include <GH_GameEngineDebug.h>
#include <map>
#include <string>


GH_GameEngineInput* GH_GameEngineInput::Instance = new GH_GameEngineInput();

GH_GameEngineInput::GH_GameEngineKey::GH_GameEngineKey()
	: down_(false), press_(false), up_(false), free_(true), key_(-1)
{

}


GH_GameEngineInput::GH_GameEngineKey::~GH_GameEngineKey()
{

}

void GH_GameEngineInput::GH_GameEngineKey::GH_Update()
{
	if (0 != GetAsyncKeyState(key_))
	{
		//GetAsyncKeyState
		//정해준 키가 눌리면 정수값을 리턴하는 함수

		//키가 눌렸을때, 현재 상태에 따라서 상태들들 바꿔주는 if문

		//방금전 프레임까지 키가 눌린적이 없을경우
		if (false == press_)
		{
			down_ = true;
			press_ = true;
			up_ = false;
			free_ = false;
			//down을 한번 true로 해준다
		}
		//직전 프레임까지 키를 누르고 있었을 경우
		else if (true == press_)
		{
			down_ = false;
			press_ = true;
			up_ = false;
			free_ = false;
			//press상태가 되고 나머지는 false
		}
	}
	else
	{
		if (true == press_)
		{
			//직전 프레임까지 키가 눌려 있었을 경우
			down_ = false;
			press_ = false;
			up_ = true;
			free_ = false;
			//up을 한번 받는다
		}
		else if (false == press_)
		{
			//up을 받은 다음의 프레임
			down_ = false;
			press_ = false;
			up_ = false;
			free_ = true;
			//원래 상태로 돌아온다
		}

	}
}

GH_GameEngineInput::GH_GameEngineInput()
{

}

GH_GameEngineInput::~GH_GameEngineInput()
{
	std::map<std::string, GH_GameEngineKey*>::iterator KeyStart = allkeys_.begin();
	std::map<std::string, GH_GameEngineKey*>::iterator KeyEnd = allkeys_.end();

	for (; KeyStart != KeyEnd; ++KeyStart)
	{
		if (nullptr == KeyStart->second)
		{
			continue;
		}

		delete KeyStart->second;
		KeyStart->second = nullptr;
	}

	allkeys_.clear();

}

GH_GameEngineInput::GH_GameEngineInput(GH_GameEngineInput&& _Other) noexcept
{
}

GH_GameEngineInput::GH_GameEngineKey* GH_GameEngineInput::GH_FindKey(const std::string _name)
{
	std::map<std::string, GH_GameEngineKey*>::iterator FindIter = allkeys_.find(_name);

	if (FindIter == allkeys_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool GH_GameEngineInput::GH_IsKey(const std::string _name)
{
	GH_GameEngineKey* FindKeyPtr = GH_FindKey(_name);

	if (nullptr == FindKeyPtr)
	{
		return false;
	}

	return true;
}

bool GH_GameEngineInput::GH_IsDown(const std::string _name)
{
	//넣어준 키가 down되엇을때의 상태를 리턴
	GH_GameEngineKey* FindKeyPtr = GH_FindKey(_name);

	if (nullptr == FindKeyPtr)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	return FindKeyPtr->down_;
}

bool GH_GameEngineInput::GH_IsUp(const std::string _name)
{

	GH_GameEngineKey* FindKeyPtr = GH_FindKey(_name);

	if (nullptr == FindKeyPtr)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	return FindKeyPtr->up_;
}

bool GH_GameEngineInput::GH_IsPress(const std::string _name)
{

	GH_GameEngineKey* FindKeyPtr = GH_FindKey(_name);

	if (nullptr == FindKeyPtr)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	return FindKeyPtr->press_;
}

bool GH_GameEngineInput::GH_IsFree(const std::string _name)
{
	//넣어준 키가 down되엇을때의 상태를 리턴
	GH_GameEngineKey* FindKeyPtr = GH_FindKey(_name);

	if (nullptr == FindKeyPtr)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	return FindKeyPtr->free_;
}

bool GH_GameEngineInput::GH_CreateKey(const std::string _name, int _key)
{
	//main에서 이용할 key를 만들때 이용하는 함수
	GH_GameEngineKey* Newkey = new GH_GameEngineKey();
	Newkey->key_ = _key;
	Newkey->GH_SetName(_name);

	allkeys_.insert(std::map<std::string, GH_GameEngineKey*>::value_type(_name, Newkey));
	return true;
}

void GH_GameEngineInput::GH_Update()
{
	//입력으로 변화된걸 프레임이 시작하기전에 변화시키는 함수
	std::map<std::string, GH_GameEngineKey*>::iterator KeyStart = allkeys_.begin();
	std::map<std::string, GH_GameEngineKey*>::iterator KeyEnd = allkeys_.end();

	for (; KeyStart != KeyEnd; ++KeyStart)
	{
		KeyStart->second->GH_Update();
	}
}