#pragma once
#include <string>
#include <Windows.h>
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
#include <vector>
#include <GH_GameEngineDebug.h>

//API���� �̿��� ��� �̹��� ���� ������ ���δ� �� Ŭ������ ���ؼ� ó���Ѵ�
//������ â(�޹��)�� �����ϴ°͵� ���Ե�


//�з� :
//�뵵 :
//���� :
class GH_ResourcesManager;
class GH_GameEngineImage : public GH_GameEngineNameBase
{
private: //����Ʈ ���� ������
	friend GH_ResourcesManager;

private:
	//���� �̹��� �������� ��� �ڵ�
	//���� �̿��Ҷ��� ����ȯ�� �̿��ϸ� �ȴ�
	HBITMAP ImageHandle_;
	HBITMAP OldHandle_;
	BITMAP ImageInfo_;

	HDC Imagedc_;

public:
	std::vector<GH_float4> CutImagePos_;
	std::vector<GH_float4> CutImageSize_;

public:
	bool GH_IsCut()
	{
		return CutImagePos_.size() != 0;
		//�� �̹����� �߷��ִ°� �ƴѰ�? �� üũ�ϴ� �Լ�
		//CutImagePos�� ũ�Ⱑ 0�� �ƴϸ� true, 0�̸�(cut()�Լ��� �߸��� �ʾѴٸ�) false
	}

	GH_float4 GH_GetCutPos(size_t _Index)
	{
		if (_Index >= CutImagePos_.size())
		{
			//�߸� �纸�ٵ� �� ū ������ ��ȣ�� �޶�� �� ���
			GH_GameEngineDebug::Assert();
			return GH_float4::ZERO;
			//��Ʈ��
		}

		return CutImagePos_[_Index];
		//�ڸ� �̹������� ���� Vector�� _Index��°�� �ִ� �̹����� �޾ƿ´�
	}

	GH_float4 GH_GetCutSize(size_t _Index)
	{
		if (_Index >= CutImageSize_.size())
		{
			GH_GameEngineDebug::Assert();
			return GH_float4::ZERO;
		}
		return CutImageSize_[_Index];
	}

public:
	HDC GH_GetDC()
	{
		return Imagedc_;
	}

	GH_float4 GH_GetSize()
	{
		return { static_cast<float>(ImageInfo_.bmWidth), static_cast<float>(ImageInfo_.bmHeight) };
		//�̹����� ����/���̸� ����
	}

private:
	GH_GameEngineImage(); //����Ʈ ������
	~GH_GameEngineImage(); //����Ʈ �Ҹ���


public:
	GH_GameEngineImage& operator=(const GH_GameEngineImage& _Other) = delete; //����Ʈ ���Կ�����
	GH_GameEngineImage& operator=(GH_GameEngineImage&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_GameEngineImage(const GH_GameEngineImage& _Other) = delete; //����Ʈ ���� ������
	GH_GameEngineImage(GH_GameEngineImage&& _Other) noexcept; //����Ʈ RValue ���������

public:
	void GH_Cut(const GH_float4 _Cut);

public:
	bool GH_Create(HDC _dc);
	bool GH_Create(HDC _dc, const GH_float4& _size);
	bool GH_Load(std::string _path);

private:
	void GH_BitMapImageInfoCheck();

public:
	void GH_BitCopy(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _Size, const GH_float4& _ImagePos = GH_float4::ZERO);
	void GH_BitCopyToImageSize(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _ImagePos = GH_float4::ZERO);
	void GH_BitCopyToImageSizeToCenter(GH_GameEngineImage* _CopyImage, const GH_float4& _Pos, const GH_float4& _ImagePos = GH_float4::ZERO);
	void GH_BitCopyToImageSizeToBottom(GH_GameEngineImage* _CopyImage, const GH_float4& _Pos, const GH_float4& _ImagePos = GH_float4::ZERO);

public:
	void GH_TransCopy(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _RenderSize, const GH_float4& _ImagePos, const GH_float4& _ImageSize, int _transcolor);

	DWORD GH_GetColor4Byte(int _x, int _y);
	GH_float4 GH_GetColorfloat4(int _x, int _y);
};

