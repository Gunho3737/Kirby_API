#pragma once
#include <string>
#include "GH_GameEngineObjectBase.h"

//�з� :
//�뵵 : ���� ����/��ü� �̸��� �����༭ ����׸� ���ϰ� �Ѵ�
//���� :

class GH_GameEngineNameBase : public GH_GameEngineObjectBase
{
private: //����Ʈ ���� ������
	std::string name_;



public:
	GH_GameEngineNameBase& operator=(const GH_GameEngineNameBase& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineNameBase& operator=(GH_GameEngineNameBase&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_GameEngineNameBase(); //����Ʈ ������
	virtual ~GH_GameEngineNameBase(); //����Ʈ �Ҹ���
	//�ڽ� Ŭ�������� �Ҹ��ڸ� �ҷ����� ���� �Ҹ����� virtualȭ

	GH_GameEngineNameBase(const GH_GameEngineNameBase& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineNameBase(GH_GameEngineNameBase&& _Other) noexcept; //����Ʈ RValue ���������

public:
	std::string GH_GetName()
	{
		return name_;
	}

	void GH_SetName(const std::string& _name)
	{
		name_ = _name;
	}
};
