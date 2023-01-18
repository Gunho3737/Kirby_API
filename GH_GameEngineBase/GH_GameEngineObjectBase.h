#pragma once
//�з� :
//�뵵 :
//���� :

class GH_GameEngineObjectBase
{
	//������Ʈ�� ���߿� ���ų�, �޸� ��ü�� �ı��ϴ� ��ɵ��� �������
private: 

	//�޸������� �ı������� ����
	bool IsDeath_;

	//������ ���� �ȵ����� ����
	bool IsUpdate_;
	
	GH_GameEngineObjectBase* parent_;

public:
	void GH_SetParent(GH_GameEngineObjectBase* _parent)
	{
		parent_ = _parent;
	}

public:
	bool GH_IsDeath()
	{
		if (nullptr == parent_)
		{
			return IsDeath_;
		}
		
		return true == IsDeath_ || parent_->IsDeath_;
	}
	
	bool GH_IsOn()
	{
		if (nullptr == parent_)
		{
			return IsUpdate_;
		}
		
		return true == IsUpdate_ && parent_->IsUpdate_;
	}

	void GH_Death()
	{
		IsDeath_ = true;
	}

	void GH_On()
	{
		IsUpdate_ = true;
	}

	void GH_Off()
	{
		IsUpdate_ = false;
	}

public:
	GH_GameEngineObjectBase& operator=(const GH_GameEngineObjectBase& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineObjectBase& operator=(GH_GameEngineObjectBase&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_GameEngineObjectBase(); //����Ʈ ������
	virtual ~GH_GameEngineObjectBase(); //����Ʈ �Ҹ��� virtual�� ������ ������ ���� ���� �ʰ� ����
	GH_GameEngineObjectBase(const GH_GameEngineObjectBase& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineObjectBase(GH_GameEngineObjectBase&& _Other) noexcept; //����Ʈ RValue ���������
	
};

