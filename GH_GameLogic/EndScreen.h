#pragma once
#include <GH_Actor.h>

//�з� :
//�뵵 :
//���� :

class EndScreen :public GH_Actor
{
private: //����Ʈ ���� ������
	GH_Renderer* MainScreen_;



public:
	EndScreen& operator=(const EndScreen& _Other) = delete; //����Ʈ ���Կ�����
	EndScreen& operator=(EndScreen&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	EndScreen(); //����Ʈ ������
	~EndScreen(); //����Ʈ �Ҹ���
	EndScreen(const EndScreen& _Other) = delete; //����Ʈ ���� ������
	EndScreen(EndScreen&& _Other) noexcept; //����Ʈ RValue ���������

public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
};


