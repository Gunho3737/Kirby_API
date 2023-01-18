#pragma once
#include "GH_GameEnginePath.h"
//분류 :
//용도 :
//설명 :

class GH_GameEngineDirectory : public GH_GameEnginePath
{
private: //디폴트 접근 지정자

public:


public:
	GH_GameEngineDirectory& operator=(const GH_GameEngineDirectory& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineDirectory& operator=(GH_GameEngineDirectory&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_GameEngineDirectory(); //디폴트 생성자
	~GH_GameEngineDirectory(); //디폴트 소멸자
	GH_GameEngineDirectory(const GH_GameEngineDirectory& _Other); //디폴트 복사 생성자
	GH_GameEngineDirectory(GH_GameEngineDirectory&& _Other) noexcept; //디폴트 RValue 복사생성자

public:
	std::string GH_DirectoryName();
	bool GH_IsExist();

public:
	bool GH_IsRoot();
	void GH_MoveParent();
	bool GH_MoveParent(std::string _DirName);
	bool GH_MoveChild(std::string _DirName);

public:
	std::string GH_CreateFileNametoPath(std::string _DirNAme);
};

