#pragma once
#include <string>
//�з� :
//�뵵 :
//���� :

class GH_GameEnginePath
{
protected: //����Ʈ ���� ������
	std::string path_;



public:
	GH_GameEnginePath& operator=(const GH_GameEnginePath& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEnginePath& operator=(GH_GameEnginePath&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_GameEnginePath(); //����Ʈ ������
	~GH_GameEnginePath(); //����Ʈ �Ҹ���
	GH_GameEnginePath(const GH_GameEnginePath& _Other); //����Ʈ ���� ������
	GH_GameEnginePath(GH_GameEnginePath&& _Other) noexcept; //����Ʈ RValue ���������

};

