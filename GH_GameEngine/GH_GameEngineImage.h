#pragma once
#include <string>
#include <Windows.h>
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
#include <vector>
#include <GH_GameEngineDebug.h>

//API에서 이용할 모든 이미지 관련 내용은 전부다 이 클래스를 통해서 처리한다
//윈도우 창(뒷배경)을 수정하는것도 포함됨


//분류 :
//용도 :
//설명 :
class GH_ResourcesManager;
class GH_GameEngineImage : public GH_GameEngineNameBase
{
private: //디폴트 접근 지정자
	friend GH_ResourcesManager;

private:
	//순수 이미지 정보만을 담는 핸들
	//직접 이용할때는 형변환을 이용하면 된다
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
		//이 이미지가 잘려있는가 아닌가? 를 체크하는 함수
		//CutImagePos의 크기가 0이 아니면 true, 0이면(cut()함수로 잘리지 않앗다면) false
	}

	GH_float4 GH_GetCutPos(size_t _Index)
	{
		if (_Index >= CutImagePos_.size())
		{
			//잘린 양보다도 더 큰 숫자의 번호를 달라고 할 경우
			GH_GameEngineDebug::Assert();
			return GH_float4::ZERO;
			//터트림
		}

		return CutImagePos_[_Index];
		//자른 이미지들이 모인 Vector의 _Index번째에 있는 이미지를 받아온다
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
		//이미지의 넓이/높이를 리턴
	}

private:
	GH_GameEngineImage(); //디폴트 생성자
	~GH_GameEngineImage(); //디폴트 소멸자


public:
	GH_GameEngineImage& operator=(const GH_GameEngineImage& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineImage& operator=(GH_GameEngineImage&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_GameEngineImage(const GH_GameEngineImage& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineImage(GH_GameEngineImage&& _Other) noexcept; //디폴트 RValue 복사생성자

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

