#pragma once
class GH_GameEngineString
{
private: //����Ʈ ���� ������

	//�з� :
	//�뵵 :
	//���� :
public:
	GH_GameEngineString& operator=(const GH_GameEngineString& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineString& operator=(GH_GameEngineString&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_GameEngineString(); //����Ʈ ������
	~GH_GameEngineString(); //����Ʈ �Ҹ���
	GH_GameEngineString(const GH_GameEngineString& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineString(GH_GameEngineString&& _Other) noexcept; //����Ʈ RValue ���������

};

