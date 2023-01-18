#pragma once
#include <string>
#include "GH_GameEngineObjectBase.h"

//분류 :
//용도 : 현재 레벨/객체등에 이름을 붙혀줘서 디버그를 편하게 한다
//설명 :

class GH_GameEngineNameBase : public GH_GameEngineObjectBase
{
private: //디폴트 접근 지정자
	std::string name_;



public:
	GH_GameEngineNameBase& operator=(const GH_GameEngineNameBase& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineNameBase& operator=(GH_GameEngineNameBase&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_GameEngineNameBase(); //디폴트 생성자
	virtual ~GH_GameEngineNameBase(); //디폴트 소멸자
	//자식 클래스들의 소멸자를 불러오기 위한 소멸자의 virtual화

	GH_GameEngineNameBase(const GH_GameEngineNameBase& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineNameBase(GH_GameEngineNameBase&& _Other) noexcept; //디폴트 RValue 복사생성자

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
