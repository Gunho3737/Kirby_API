#pragma once
#include <GH_GameEngineNameBase.h>
#include <map>
#include <string>

//�з� :
//�뵵 :
//���� :
class GH_GameEngineWindow;
class GH_GameEngineInput
{
private:
	friend GH_GameEngineWindow;

private: //����Ʈ ���� ������
	class GH_GameEngineKey : public GH_GameEngineNameBase
	{
		friend GH_GameEngineInput;

	private:

		bool down_; //Ű�� ������ �� �ѹ�
		bool press_; //Ű�� ���������� ������ ������
		bool up_; //Ű�� ������ �ʰ� ������ �ѹ�
		bool free_; //Ű�� ������ ���� ����
		int key_; //���� �������� Ű�� �ε��� Ȥ�� Ÿ��

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
	GH_GameEngineInput& operator=(const GH_GameEngineInput& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineInput& operator=(GH_GameEngineInput&& _Other) = delete; //����Ʈ RValue ���Կ�����
private:
	GH_GameEngineInput(); //����Ʈ ������
	~GH_GameEngineInput(); //����Ʈ �Ҹ���
public:
	GH_GameEngineInput(const GH_GameEngineInput& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineInput(GH_GameEngineInput&& _Other) noexcept; //����Ʈ RValue ���������


public:
	//�����쿡�� Ư��Ű�� ��밡���ϰ� ����� �Լ�
	bool GH_CreateKey(const std::string _name, int _key);

	//���� Ư��Ű�� Ư���� ���¿� �ִ°�? �� bool�� üũ�ϴ� �Լ�
	bool GH_IsDown(const std::string _name); //Ű�� ���Ǵ°�
	bool GH_IsUp(const std::string _name); //Ű�� �������°�
	bool GH_IsPress(const std::string _name); //Ű�� ������ �ִ°�
	bool GH_IsFree(const std::string _name); //Ű�� �������� �ִ� �����ΰ�

	//���� ���� Ű�� ����� ���°�?�� üũ�ϴ� �Լ�
	bool GH_IsKey(const std::string _name);

private:
	GH_GameEngineKey* GH_FindKey(const std::string _name);
	void GH_Update();
};

