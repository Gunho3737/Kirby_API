#pragma once
#include <GH_Actor.h>
#include "GameLogicEnum.h"

//�з� :
//�뵵 :
//���� :
class Stage_one_one;
class One_One_Far : public GH_Actor
{
	friend Stage_one_one;
		
public:
	One_One_Far& operator=(const One_One_Far& _Other) = delete; //����Ʈ ���Կ�����
	One_One_Far& operator=(One_One_Far&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	One_One_Far(); //����Ʈ ������
	~One_One_Far(); //����Ʈ �Ҹ���
	One_One_Far(const One_One_Far& _Other) = delete; //����Ʈ ���� ������
	One_One_Far(One_One_Far&& _Other) noexcept; //����Ʈ RValue ���������

public:
	void GH_Update() override;
	//��ġ���� TitleSize_�� ��ȭ��Ű�� Update

	void GH_Render() override;
	//��ġ���� TitleSize_�� �޾Ƽ� ���� ��ġ�� �������� �׸��� �׸�

	void GH_Start() override;

private:
	StageState MapState;


};