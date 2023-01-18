#pragma once
#include <string>
//분류 :
//용도 :
//설명 :

class GH_GameEnginePath
{
protected: //디폴트 접근 지정자
	std::string path_;



public:
	GH_GameEnginePath& operator=(const GH_GameEnginePath& _Other) = delete; //디폴트 대입연산자
	GH_GameEnginePath& operator=(GH_GameEnginePath&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_GameEnginePath(); //디폴트 생성자
	~GH_GameEnginePath(); //디폴트 소멸자
	GH_GameEnginePath(const GH_GameEnginePath& _Other); //디폴트 복사 생성자
	GH_GameEnginePath(GH_GameEnginePath&& _Other) noexcept; //디폴트 RValue 복사생성자

};

