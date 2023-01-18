#pragma once
#include <map>
#include <string>
#include <GH_GameEngineDebug.h>
#include "GH_Level.h"

//�з� : �Ŵ���(����Ŭ����)
//�뵵 : ��������
//���� : �����������
//       ������ ��ȯ������ �� �ִ� ����� ���
//class GH_Level;
class GH_LevelManager
{
private:
	static GH_LevelManager* Instance;

private:
	std::map<std::string, GH_Level*> LevelMap_;
	//���ڿ� key�� ����� Level �ڷ���� �����ϴ� map

	GH_Level* CurrentLevel_;
	//���� ���� ���� ����(���)�� �ִ����� �����ϴ� ����

	GH_Level* NextLevel_;
	//���� ������ �����ϴ� ����

private:
	GH_LevelManager(); //����Ʈ ������
	~GH_LevelManager(); //����Ʈ �Ҹ���

public:
	GH_LevelManager& operator=(const GH_LevelManager& _Other) = delete; //����Ʈ ���Կ�����
	GH_LevelManager& operator=(GH_LevelManager&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_LevelManager(const GH_LevelManager& _Other) = delete; //����Ʈ ���� ������
	GH_LevelManager(GH_LevelManager&& _Other) noexcept; //����Ʈ RValue ���������
public:
	static GH_LevelManager& GH_GetInst()
	{
		return *Instance;
		//static �̹Ƿ� .h�� ������� �ߴ�
		//static�� �̿��� �̱���
	}

	static void GH_Destory()
	{
		if (nullptr != Instance)
		{
			delete Instance;
			Instance = nullptr;
			//���� �Ҵ� �����Ƿ� ���� �ȳ��� delete �������

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
			//�̹� ������� �̸��� �����̸� ��Ʈ����
		}

		GH_Level* newLevel = new LevelType();
		//�ڽ� Ŭ������ �ڷ����� ���缭 �����Ҵ�
		newLevel->GH_Levelloading();
		//���� ������ �ʿ��� actor���� ������
		//virtual�� �̿��ؼ� �ڽ��� �Լ��� �����Ŵ

		newLevel->GH_SetName(_LVname);
		//������ ��������, ��ġ�� ���ϰ� �ϱ� ���� �̸��� �������

		LevelMap_.insert(std::map<std::string, GH_Level*>::value_type(_LVname, newLevel));
		//������ map�� �����Ѵ�
	}


	void GH_ChangeLevel(std::string _LVname);

	void GH_Update();
};

