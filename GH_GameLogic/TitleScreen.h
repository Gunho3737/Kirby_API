#pragma once
#include <GH_Actor.h>

//�з� :
//�뵵 :
//���� :

class TitleScreen :public GH_Actor
{
private: //����Ʈ ���� ������
	GH_Renderer* MainScreen_;
	GH_Renderer* StartRender_;
	static float TimeCheck_;
	
public:
	TitleScreen& operator=(const TitleScreen& _Other) = delete; //����Ʈ ���Կ�����
	TitleScreen& operator=(TitleScreen&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	TitleScreen(); //����Ʈ ������
	~TitleScreen(); //����Ʈ �Ҹ���
	TitleScreen(const TitleScreen& _Other) = delete; //����Ʈ ���� ������
	TitleScreen(TitleScreen&& _Other) noexcept; //����Ʈ RValue ���������
	
public:
	void GH_Update() override;
	void GH_Render() override;
	void GH_Start() override;
};

