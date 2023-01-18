#pragma once
#include <GH_Actor.h>
//�з� :
//�뵵 :
//���� :
class Normal_Kirby;
class InGame_UI : public GH_Actor
{
	friend Normal_Kirby;
private: //����Ʈ ���� ������
	GH_Renderer* HPBarRender_;
	GH_Renderer* KirbyAbilityRender_;
	GH_Renderer* BossHPBarRender_;
	
public:
	InGame_UI& operator=(const InGame_UI& _Other) = delete; //����Ʈ ���Կ�����
	InGame_UI& operator=(InGame_UI&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	InGame_UI(); //����Ʈ ������
	~InGame_UI(); //����Ʈ �Ҹ���
public:
	InGame_UI(const InGame_UI& _Other) = delete; //����Ʈ ���� ������
	InGame_UI(InGame_UI&& _Other) noexcept; //����Ʈ RValue ���������
	
public:
	void GH_Update() override;
	//��ġ���� TitleSize_�� ��ȭ��Ű�� Update

	void GH_Render() override;
	//��ġ���� TitleSize_�� �޾Ƽ� ���� ��ġ�� �������� �׸��� �׸�

	void GH_Start() override;
};

