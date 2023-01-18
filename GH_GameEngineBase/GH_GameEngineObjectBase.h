#pragma once
//분류 :
//용도 :
//설명 :

class GH_GameEngineObjectBase
{
	//오브젝트를 도중에 끄거나, 메모리 자체를 파괴하는 기능들이 들어있음
private: 

	//메모리적으로 파괴할지의 여부
	bool IsDeath_;

	//실행이 될지 안될지의 여부
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
	GH_GameEngineObjectBase& operator=(const GH_GameEngineObjectBase& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineObjectBase& operator=(GH_GameEngineObjectBase&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_GameEngineObjectBase(); //디폴트 생성자
	virtual ~GH_GameEngineObjectBase(); //디폴트 소멸자 virtual을 붙히는 것으로 릭이 남지 않게 만듬
	GH_GameEngineObjectBase(const GH_GameEngineObjectBase& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineObjectBase(GH_GameEngineObjectBase&& _Other) noexcept; //디폴트 RValue 복사생성자
	
};

