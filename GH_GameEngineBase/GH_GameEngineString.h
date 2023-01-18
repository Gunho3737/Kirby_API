#pragma once
class GH_GameEngineString
{
private: //디폴트 접근 지정자

	//분류 :
	//용도 :
	//설명 :
public:
	GH_GameEngineString& operator=(const GH_GameEngineString& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineString& operator=(GH_GameEngineString&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_GameEngineString(); //디폴트 생성자
	~GH_GameEngineString(); //디폴트 소멸자
	GH_GameEngineString(const GH_GameEngineString& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineString(GH_GameEngineString&& _Other) noexcept; //디폴트 RValue 복사생성자

};

