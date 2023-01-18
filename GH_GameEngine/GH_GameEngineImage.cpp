#pragma comment(lib, "msimg32.lib")

#include "GH_GameEngineImage.h"
#include <GH_GameEngineDebug.h>


GH_GameEngineImage::GH_GameEngineImage()
{

}

GH_GameEngineImage::~GH_GameEngineImage()
{
}

GH_GameEngineImage::GH_GameEngineImage(GH_GameEngineImage&& _Other) noexcept
{
}

void GH_GameEngineImage::GH_Cut(const GH_float4 _cut)
{
	if (0.0f >= _cut.x)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	if (0.0f >= _cut.y)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	size_t XCount = static_cast<size_t>(GH_GetSize().x / _cut.x);
	size_t YCount = static_cast<size_t>(GH_GetSize().y / _cut.y);

	CutImagePos_.reserve(XCount * YCount);
	CutImageSize_.reserve(XCount * YCount);

	GH_float4 StartImagePos = GH_float4::ZERO;
	GH_float4 StartImageSize = _cut;

	for (size_t i = 0; i < YCount; i++)
	{
		for (size_t i = 0; i < XCount; i++)
		{
			CutImagePos_.push_back(StartImagePos);
			CutImageSize_.push_back(StartImageSize);
			//���� �־��� ũ�⸸ŭ �̹����� �ҷ����

			StartImagePos.x += StartImageSize.x;
			//0,0 ���� ���� �־��� ũ�⸸ŭ �̹��� ���� ��ǥ�� �Ű��
			//�Ű��� ũ�⸸ŭ �̵��� �ٽ� �̹���ũ�⸸ŭ �ҷ����°�����
			//������ �������� �̹����� �߶� vector�� ������ �� �ְԵ�
		}

		StartImagePos.x = 0.0f;
		StartImagePos.y += StartImageSize.y;
		//y���� �Ѵܰ� �ٲ��
	}

}

bool GH_GameEngineImage::GH_Load(std::string _path)
{
	//���� �̿��� bmp �̹����� ���� ���� �������� �Լ�

	HANDLE LoadHandle = nullptr;


	LoadHandle = LoadImageA(nullptr //�ڵ��� ����, bmp�� ������ ��� nullptr�� �ִ´�
		, _path.c_str() //�ּҰ������� char*�� �޾ƿ�
		, IMAGE_BITMAP //bmp�ϰ�� IMAGE_BITMAP
		, 0, 0
		, LR_LOADFROMFILE);
	//�� �̹���bmp�� �ּҸ� �޾Ƽ� �� �̹����� ���� ������ �����ϰ�, �ű⿡ �����Ҽ� �ִ� �ڵ��� ���Ϲ���

	ImageHandle_ = static_cast<HBITMAP>(LoadHandle);

	if (nullptr == ImageHandle_)
	{
		GH_GameEngineDebug::Assert();
		return false;
		//�ּҿ��� �̹����� �ҷ����� ������ ���, ��Ʈ��
	}

	Imagedc_ = CreateCompatibleDC(nullptr);
	//nullptr�� ����Ű���ִ� ���ο� HDC�ڵ��� ����
	HANDLE Oldhandle = SelectObject(Imagedc_, static_cast<HGDIOBJ>(ImageHandle_));
	OldHandle_ = static_cast<HBITMAP>(Oldhandle);

	GH_BitMapImageInfoCheck();

	return true;

}

bool GH_GameEngineImage::GH_Create(HDC _dc)
{
	if (nullptr == _dc)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	Imagedc_ = _dc;

	GH_BitMapImageInfoCheck();
	return true;
}

bool GH_GameEngineImage::GH_Create(HDC _dc, const GH_float4& _size)
{
	ImageHandle_ = CreateCompatibleBitmap(_dc, _size.ix(), _size.iy());


	Imagedc_ = CreateCompatibleDC(nullptr);
	// 1,1 ũ���� ���׸� bmp�����̶� ����� DC�� ����	
	HANDLE Oldhandle = SelectObject(Imagedc_, static_cast<HGDIOBJ>(ImageHandle_));
	//Imagedc_ �� ImageHandle_ �� ����Ǿ� �ִ� bmp ���ϳ��� ���� �ٲ�
	OldHandle_ = static_cast<HBITMAP>(Oldhandle);

	GH_BitMapImageInfoCheck();

	return true;


}

void GH_GameEngineImage::GH_BitMapImageInfoCheck()
{
	if (nullptr == Imagedc_)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	// ������� �Դٸ� �̹��� �ڵ��� �̹� �����ϴ� �����̾�� �Ѵ�
	// imagehandle_�� ���ؼ� �׳� �ᵵ �ȴ�.

	HBITMAP CheckBitMap = static_cast<HBITMAP>(GetCurrentObject(Imagedc_, OBJ_BITMAP));
	GetObjectA(CheckBitMap, sizeof(BITMAP), &ImageInfo_);

}

void GH_GameEngineImage::GH_BitCopyToImageSize(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _ImagePos)
{
	GH_BitCopy(_CopyImage, _LeftTopPos, _CopyImage->GH_GetSize(), _ImagePos);
	//�̹� �̹����� ũ�������� ������ �ִ� ����� BitCopy
}

void GH_GameEngineImage::GH_BitCopy(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _Size, const GH_float4& _ImagePos /*= GH_float4::ZERO*/)
{
	BitBlt(Imagedc_,
		_LeftTopPos.ix(), //const _LeftTopPos��, ix()�� ���� �Լ����� const���°� �ƴϸ� ���� ����
		_LeftTopPos.iy(),
		_Size.ix(),
		_Size.iy(),
		_CopyImage->Imagedc_,
		_ImagePos.ix(),
		_ImagePos.iy(),
		SRCCOPY);

	//�־��� �̹����� ����/���縦 ������ �������� ��ǥ/�� ������â���� �Ҵ����� ũ�������� �޾Ƽ�
	//�����Ű�� �Լ�
}

void  GH_GameEngineImage::GH_BitCopyToImageSizeToCenter(GH_GameEngineImage* _CopyImage, const GH_float4& _Pos, const GH_float4& _ImagePos /*= GH_float4::ZERO*/)
{

	GH_BitCopy(_CopyImage, _Pos - _CopyImage->GH_GetSize().GH_halffloat4(), _CopyImage->GH_GetSize(), _ImagePos);
	// _CopyImage->GH_GetSize().GH_halffloat4() �̹����� ũ�������� �޾Ƽ� �� ��� ��ǥ���� �޾ƿ�
	// => BitBlt �Լ��� �̿��Ϸ��� _Pos�� ���� ���������� ����Ѵ�
	// ���⼭�� Actor�� ��ġ�� �̹����� ������ �����ѵ�, Actor�� ��ǥ�� �������� �̹����� ������� �������� ��ǥ�� �Լ��� �־��ذ�
}


void GH_GameEngineImage::GH_BitCopyToImageSizeToBottom(GH_GameEngineImage* _CopyImage, const GH_float4& _Pos, const GH_float4& _ImagePos /*= GH_float4::ZERO*/)
{
	GH_float4 Center = _CopyImage->GH_GetSize().GH_halffloat4();
	Center.y = _CopyImage->GH_GetSize().y;
	GH_BitCopy(_CopyImage, _Pos - Center, _CopyImage->GH_GetSize(), _ImagePos);
}

void GH_GameEngineImage::GH_TransCopy(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _RenderSize, const GH_float4& _ImagePos, const GH_float4& _ImageSize, int _transcolor)
{
	//�̹����� �����ؼ� �������°�
	//Ư�� ���� �����ϰ� �������� �� �� �ִ�=> �����̹��� ����
	//ũ������ ���� ������
	//ũ�������� ��� ������ �߰��� ���� ���Ƿ� ���� �ʴ°��� ��õ
	TransparentBlt(Imagedc_,
		_LeftTopPos.ix(), //LeftTopPos => �� ��ġ�������� �׸��� ����
		_LeftTopPos.iy(), //LeftTopPos => �� ��ġ�������� �׸��� ����
		_RenderSize.ix(), //RenderSize=> �̹����� �� ��ġ��������
		_RenderSize.iy(), //RenderSize=> �̹����� �� ��ġ��������
		_CopyImage->Imagedc_,
		_ImagePos.ix(), //_ImagePos => RenderSize���� ImagePos������ ��ġ�� �׸���
		_ImagePos.iy(), //_ImagePos => RenderSize���� ImagePos������ ��ġ�� �׸���
		_ImageSize.ix(), //_ImageSize => ȭ�鿡 �̸��� ũ��� �׷���
		_ImageSize.iy(), //_ImageSize => ȭ�鿡 �̸��� ũ��� �׷���
		_transcolor // �� ���� �����ϰ�
	);

	//�ִϸ��̼��� �۵�����
	//1. cut �Լ��� �̿��ؼ� �̹����� ũ��/��ġ�� 

}

DWORD GH_GameEngineImage::GH_GetColor4Byte(int _x, int _y)
{
	return GetPixel(Imagedc_, _x, _y);
	//Ư�� ��ġ�� �ȼ��� �������� ������ �´�
}
GH_float4 GH_GameEngineImage::GH_GetColorfloat4(int _x, int _y)
{
	COLORREF ColorValue = GetPixel(Imagedc_, _x, _y);
	//Ư�� ��ġ�� �ȼ��� �������� ������ �´�

	unsigned char* ptr = reinterpret_cast<unsigned char*>(&ColorValue);
	//COLORREF(unsigned Long) �� ColorValue�� 1����Ʈ ������ �ɰ��� ���� �Ҽ� �ְ� ����

	GH_float4 Colorfloat4;
	
	//1����Ʈ�� �����ϴ� ������ rgba���� ���� ���� ����
	Colorfloat4.r = ptr[0] / 255.0f; //r
	Colorfloat4.g = ptr[1] / 255.0f;
	Colorfloat4.b = ptr[2] / 255.0f;
	Colorfloat4.a = ptr[3] / 255.0f;

	return Colorfloat4;
}