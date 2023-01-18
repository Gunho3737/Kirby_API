#pragma once
//�з� : �����
//�뵵 : �޸𸮰��� �޼������ �α�
//���� : ��� ����� ���� ���⿡ ��� ���´�.

class GH_GameEngineDebug
{
private: //����Ʈ ���� ������
	GH_GameEngineDebug(); //����Ʈ ������
	~GH_GameEngineDebug(); //����Ʈ �Ҹ���

private:
	GH_GameEngineDebug& operator=(const GH_GameEngineDebug& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineDebug& operator=(GH_GameEngineDebug&& _Other) = delete; //����Ʈ RValue ���Կ�����
private:

	GH_GameEngineDebug(const GH_GameEngineDebug& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineDebug(GH_GameEngineDebug&& _Other) noexcept; //����Ʈ RValue ���������

public:
	//��� �Լ��� static���� ���
	static void MessageBoxError(const char* _Ptr);
	static void Assert();
	static void GH_LeakCheckOn();
};