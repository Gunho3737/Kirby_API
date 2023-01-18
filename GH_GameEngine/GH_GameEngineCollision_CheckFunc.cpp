#include "GH_GameEngineCollision.h"
#include <GH_GameEngineDebug.h>
#include "GH_GameEngineImage.h"

bool (*GH_GameEngineCollision::GH_arrFunc_[static_cast<int>(GH_CollisionCheckType::MAX)][static_cast<int>(GH_CollisionCheckType::MAX)])(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol);

void GH_GameEngineCollision::GH_ColCheckFuncInitialize()
{

	for (size_t y = 0; y < static_cast<int>(GH_CollisionCheckType::MAX); y++)
	{
		for (size_t x = 0; x < static_cast<int>(GH_CollisionCheckType::MAX); x++)
		{
			GH_arrFunc_[y][x] = nullptr;
		}

	}

	//�浹 Ÿ�԰� �´� �迭�ڸ��� �Լ������͸� ����ִ´�
	GH_arrFunc_[static_cast<int>(GH_CollisionCheckType::POINT)][static_cast<int>(GH_CollisionCheckType::IMAGE)] = &GH_GameEngineCollision::GH_PointToImage;
	GH_arrFunc_[static_cast<int>(GH_CollisionCheckType::IMAGE)][static_cast<int>(GH_CollisionCheckType::POINT)] = &GH_GameEngineCollision::GH_ImageToPoint;
	GH_arrFunc_[static_cast<int>(GH_CollisionCheckType::RECTANGLE)][static_cast<int>(GH_CollisionCheckType::RECTANGLE)] = &GH_GameEngineCollision::GH_RectToRect;
}

//��� �浹������ ���⼭ ���� �Լ����� �ҷ����� �����̸�
//�ʿ��Ҷ����� ���� xxtoxx�� ���� �����߸� �浹�� �۵��Ѵ�

bool GH_GameEngineCollision::GH_PointToImage(GH_GameEngineCollision* _PointCol, GH_GameEngineCollision* _ImageCol)
{
	GH_float4 Pos = _PointCol->GH_GetCollisionPos();

	if (nullptr == _ImageCol->ImagePtr_)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	COLORREF Color = _ImageCol->ImagePtr_->GH_GetColor4Byte(Pos.ix(), Pos.iy());

	//���� ������ ���� ���ٸ� true, �ƴϸ� false
	return _PointCol->GH_GetColorCheck() == Color;
}

bool GH_GameEngineCollision::GH_ImageToPoint(GH_GameEngineCollision* _ImageCol, GH_GameEngineCollision* _PointCol)
{
	return GH_PointToImage(_PointCol, _ImageCol);
}

bool GH_GameEngineCollision::GH_RectToRect(GH_GameEngineCollision* _Left, GH_GameEngineCollision* _Right)
{
	GH_Figure Left = _Left->GH_GetFigure();
	//������� ũ�⸦ ���� _Left�� �����ؿ�
	GH_Figure Right = _Right->GH_GetFigure();

	if (Left.iRight() < Right.iLeft())
	{
		return false;
	}

	if (Left.iLeft() > Right.iRight())
	{
		return false;
	}

	if (Left.iTop() > Right.iBot())
	{
		return false;
	}

	if (Left.iBot() < Right.iTop())
	{
		return false;
	}

	//���� �浹���� ���� ����� ���� �����ϸ� true�� �ȴ�
	return true;
}

