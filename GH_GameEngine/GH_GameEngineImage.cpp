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
			//내가 넣어준 크기만큼 이미지를 불러운다

			StartImagePos.x += StartImageSize.x;
			//0,0 에서 내가 넣어준 크기만큼 이미지 시작 좌표를 옮겼다
			//옮겨진 크기만큼 이동후 다시 이미지크기만큼 불러오는것으로
			//일정한 간격으로 이미지를 잘라서 vector에 저장할 수 있게됨
		}

		StartImagePos.x = 0.0f;
		StartImagePos.y += StartImageSize.y;
		//y축을 한단계 바꿨다
	}

}

bool GH_GameEngineImage::GH_Load(std::string _path)
{
	//내가 이용할 bmp 이미지의 색상 값을 가져오는 함수

	HANDLE LoadHandle = nullptr;


	LoadHandle = LoadImageA(nullptr //핸들을 넣음, bmp를 실행할 경우 nullptr을 넣는다
		, _path.c_str() //주소값정보를 char*로 받아옴
		, IMAGE_BITMAP //bmp일경우 IMAGE_BITMAP
		, 0, 0
		, LR_LOADFROMFILE);
	//내 이미지bmp의 주소를 받아서 그 이미지에 대한 정보를 저장하고, 거기에 접근할수 있는 핸들을 리턴받음

	ImageHandle_ = static_cast<HBITMAP>(LoadHandle);

	if (nullptr == ImageHandle_)
	{
		GH_GameEngineDebug::Assert();
		return false;
		//주소에서 이미지를 불러오지 못햇을 경우, 터트림
	}

	Imagedc_ = CreateCompatibleDC(nullptr);
	//nullptr을 가리키고있는 새로운 HDC핸들을 만듬
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
	// 1,1 크기의 조그만 bmp파일이랑 연결된 DC를 만듬	
	HANDLE Oldhandle = SelectObject(Imagedc_, static_cast<HGDIOBJ>(ImageHandle_));
	//Imagedc_ 와 ImageHandle_ 이 연결되어 있는 bmp 파일끼리 서로 바꿈
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

	// 여기까지 왔다면 이미지 핸들이 이미 존재하는 시점이어야 한다
	// imagehandle_을 통해서 그냥 써도 된다.

	HBITMAP CheckBitMap = static_cast<HBITMAP>(GetCurrentObject(Imagedc_, OBJ_BITMAP));
	GetObjectA(CheckBitMap, sizeof(BITMAP), &ImageInfo_);

}

void GH_GameEngineImage::GH_BitCopyToImageSize(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _ImagePos)
{
	GH_BitCopy(_CopyImage, _LeftTopPos, _CopyImage->GH_GetSize(), _ImagePos);
	//이미 이미지의 크기정보를 가지고 있는 경우의 BitCopy
}

void GH_GameEngineImage::GH_BitCopy(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _Size, const GH_float4& _ImagePos /*= GH_float4::ZERO*/)
{
	BitBlt(Imagedc_,
		_LeftTopPos.ix(), //const _LeftTopPos라, ix()와 같은 함수들이 const상태가 아니면 받지 못함
		_LeftTopPos.iy(),
		_Size.ix(),
		_Size.iy(),
		_CopyImage->Imagedc_,
		_ImagePos.ix(),
		_ImagePos.iy(),
		SRCCOPY);

	//넣어줄 이미지의 정보/복사를 시작할 시작점의 좌표/내 윈도우창에서 할당해줄 크기정보를 받아서
	//복사시키는 함수
}

void  GH_GameEngineImage::GH_BitCopyToImageSizeToCenter(GH_GameEngineImage* _CopyImage, const GH_float4& _Pos, const GH_float4& _ImagePos /*= GH_float4::ZERO*/)
{

	GH_BitCopy(_CopyImage, _Pos - _CopyImage->GH_GetSize().GH_halffloat4(), _CopyImage->GH_GetSize(), _ImagePos);
	// _CopyImage->GH_GetSize().GH_halffloat4() 이미지의 크기정보를 받아서 그 가운데 좌표값을 받아옴
	// => BitBlt 함수를 이용하려면 _Pos를 제일 왼쪽점으로 줘야한다
	// 여기서는 Actor의 위치가 이미지의 가운데라고 가정한뒤, Actor의 좌표를 기준으로 이미지의 가장왼쪽 꼭지점의 좌표를 함수에 넣어준것
}


void GH_GameEngineImage::GH_BitCopyToImageSizeToBottom(GH_GameEngineImage* _CopyImage, const GH_float4& _Pos, const GH_float4& _ImagePos /*= GH_float4::ZERO*/)
{
	GH_float4 Center = _CopyImage->GH_GetSize().GH_halffloat4();
	Center.y = _CopyImage->GH_GetSize().y;
	GH_BitCopy(_CopyImage, _Pos - Center, _CopyImage->GH_GetSize(), _ImagePos);
}

void GH_GameEngineImage::GH_TransCopy(GH_GameEngineImage* _CopyImage, const GH_float4& _LeftTopPos, const GH_float4& _RenderSize, const GH_float4& _ImagePos, const GH_float4& _ImageSize, int _transcolor)
{
	//이미지를 가공해서 가져오는것
	//특정 색을 제외하고 렌더링을 할 수 있다=> 투명이미지 가능
	//크기조정 역시 가능함
	//크기조정의 경우 연산이 추가로 많이 들어가므로 하지 않는것을 추천
	TransparentBlt(Imagedc_,
		_LeftTopPos.ix(), //LeftTopPos => 이 위치에서부터 그리기 시작
		_LeftTopPos.iy(), //LeftTopPos => 이 위치에서부터 그리기 시작
		_RenderSize.ix(), //RenderSize=> 이미지의 이 위치에서부터
		_RenderSize.iy(), //RenderSize=> 이미지의 이 위치에서부터
		_CopyImage->Imagedc_,
		_ImagePos.ix(), //_ImagePos => RenderSize부터 ImagePos까지의 위치의 그림을
		_ImagePos.iy(), //_ImagePos => RenderSize부터 ImagePos까지의 위치의 그림을
		_ImageSize.ix(), //_ImageSize => 화면에 이만한 크기로 그려라
		_ImageSize.iy(), //_ImageSize => 화면에 이만한 크기로 그려라
		_transcolor // 이 색을 제외하고
	);

	//애니메이션의 작동원리
	//1. cut 함수를 이용해서 이미지의 크기/위치를 

}

DWORD GH_GameEngineImage::GH_GetColor4Byte(int _x, int _y)
{
	return GetPixel(Imagedc_, _x, _y);
	//특정 위치의 픽셀의 색정보를 가지고 온다
}
GH_float4 GH_GameEngineImage::GH_GetColorfloat4(int _x, int _y)
{
	COLORREF ColorValue = GetPixel(Imagedc_, _x, _y);
	//특정 위치의 픽셀의 색정보를 가지고 온다

	unsigned char* ptr = reinterpret_cast<unsigned char*>(&ColorValue);
	//COLORREF(unsigned Long) 인 ColorValue를 1바이트 단위로 쪼개서 접근 할수 있게 만듬

	GH_float4 Colorfloat4;
	
	//1바이트씩 접근하는 것으로 rgba값을 받을 수가 있음
	Colorfloat4.r = ptr[0] / 255.0f; //r
	Colorfloat4.g = ptr[1] / 255.0f;
	Colorfloat4.b = ptr[2] / 255.0f;
	Colorfloat4.a = ptr[3] / 255.0f;

	return Colorfloat4;
}