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
		//������ Ű�� ������ �������� �����ϴ� �Լ�

		//Ű�� ��������, ���� ���¿� ���� ���µ�� �ٲ��ִ� if��

		//����� �����ӱ��� Ű�� �������� �������
		if (false == press_)
		{
			down_ = true;
			press_ = true;
			up_ = false;
			free_ = false;
			//down�� �ѹ� true�� ���ش�
		}
		//���� �����ӱ��� Ű�� ������ �־��� ���
		else if (true == press_)
		{
			down_ = false;
			press_ = true;
			up_ = false;
			free_ = false;
			//press���°� �ǰ� �������� false
		}
	}
	else
	{
		if (true == press_)
		{
			//���� �����ӱ��� Ű�� ���� �־��� ���
			down_ = false;
			press_ = false;
			up_ = true;
			free_ = false;
			//up�� �ѹ� �޴´�
		}
		else if (false == press_)
		{
			//up�� ���� ������ ������
			down_ = false;
			press_ = false;
			up_ = false;
			free_ = true;
			//���� ���·� ���ƿ´�
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
	//�־��� Ű�� down�Ǿ������� ���¸� ����
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
	//�־��� Ű�� down�Ǿ������� ���¸� ����
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
	//main���� �̿��� key�� ���鶧 �̿��ϴ� �Լ�
	GH_GameEngineKey* Newkey = new GH_GameEngineKey();
	Newkey->key_ = _key;
	Newkey->GH_SetName(_name);

	allkeys_.insert(std::map<std::string, GH_GameEngineKey*>::value_type(_name, Newkey));
	return true;
}

void GH_GameEngineInput::GH_Update()
{
	//�Է����� ��ȭ�Ȱ� �������� �����ϱ����� ��ȭ��Ű�� �Լ�
	std::map<std::string, GH_GameEngineKey*>::iterator KeyStart = allkeys_.begin();
	std::map<std::string, GH_GameEngineKey*>::iterator KeyEnd = allkeys_.end();

	for (; KeyStart != KeyEnd; ++KeyStart)
	{
		KeyStart->second->GH_Update();
	}
}