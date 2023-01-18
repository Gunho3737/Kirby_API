#pragma once
#include "GH_GameEnginePath.h"
//�з� :
//�뵵 :
//���� :

class GH_GameEngineDirectory : public GH_GameEnginePath
{
private: //����Ʈ ���� ������

public:


public:
	GH_GameEngineDirectory& operator=(const GH_GameEngineDirectory& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineDirectory& operator=(GH_GameEngineDirectory&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_GameEngineDirectory(); //����Ʈ ������
	~GH_GameEngineDirectory(); //����Ʈ �Ҹ���
	GH_GameEngineDirectory(const GH_GameEngineDirectory& _Other); //����Ʈ ���� ������
	GH_GameEngineDirectory(GH_GameEngineDirectory&& _Other) noexcept; //����Ʈ RValue ���������

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

