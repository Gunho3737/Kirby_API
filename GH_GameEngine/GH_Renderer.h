#pragma once
#include <map>
#include <string>
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
//분류 : 렌더링
//용도 : 그림을 그리는데 필요한 편의기능 제공
//설명 : 액터가 관리/렌더링을 필요한 기능을 제공한다

// 사용자 정의 자료형
// 보통 상태(하나의 객체가 몇가지 상태를 가진다고 할때)를 
// 명시적으로 표현하고 싶을때 많이 사용
enum class GH_RENDERPIVOT
{	//switch에서 0,1,2,3 등으로 하기보다
	//명시적으로 알기 쉬운단어들을 이용
	//string으로 switch를 하는것 대신 이용하는것
	CENTER,
	BOT,
	LEFTTOP,
	MAX
};


class GH_Actor;
class GH_GameEngineImage;
class GH_Renderer : public GH_GameEngineNameBase
{
private:
	class GH_FrameAnimation : public GH_GameEngineNameBase
	{
		friend GH_Renderer;

	private:
		int Start_;
		int End_;
		int Current_;
		float FrameTime_;
		float CurFrameTime_;
		GH_Renderer* Renderer_;
		GH_GameEngineImage* Image_;
		GH_RENDERPIVOT Pivot_;
		bool Loop_;
		bool IsEnd_;

	private:
		GH_FrameAnimation();

	private:
		void GH_Reset()
		{
			Current_ = Start_;
			CurFrameTime_ = FrameTime_;
		}
		void GH_Update();
	};

private: //디폴트 접근 지정자
	friend GH_Actor;

private:
	GH_Actor* Actor_;
	GH_float4 ImagePivot_; //이미지의 중심이 될 좌표=> 취향에 따라 발에 두던, 중앙에 두던, 0,0에 두던 자유
	GH_float4 Pivotpos_; //기본 중심점에서 렌더 위치를 바꾸고 싶을때 더해주는 값을 저장한 변수
	GH_float4 Rendersize_; //렌더를 시작할 좌표
	GH_float4 Imagepos_; //들여올 이미지의 시작점
	GH_float4 ImageSize_;//이미지의 크기
	GH_GameEngineImage* Image_;

	bool IsCamEffect_;

private:
	void GH_SetActor(GH_Actor* _Actor)
	{
		Actor_ = _Actor;
	}

public:
	void GH_SetImagePivot(const GH_float4& _Value)
	{
		ImagePivot_ = _Value;
	}

	void GH_SetPivotPos(const GH_float4& _Value)
	{
		Pivotpos_ = _Value;
	}

	void GH_SetRenderSize(const GH_float4& _Value)
	{
		Rendersize_ = _Value;
	}

	void GH_SetImagePos(const GH_float4& _Value)
	{
		Imagepos_ = _Value;
	}

	void GH_SetImageSize(const GH_float4& _Value)
	{
		ImageSize_ = _Value;
	}

	void GH_SetImage(const std::string& _Name);

	GH_GameEngineImage* GH_GetImage()
	{
		return Image_;
	}

public:
	GH_Renderer& operator=(const GH_Renderer& _Other) = delete; //디폴트 대입연산자
	GH_Renderer& operator=(GH_Renderer&& _Other) = delete; //디폴트 RValue 대입연산자
public:
	GH_Renderer(); //디폴트 생성자
	~GH_Renderer(); //디폴트 소멸자
	GH_Renderer(const GH_Renderer& _Other) = delete; //디폴트 복사 생성자
	GH_Renderer(GH_Renderer&& _Other) noexcept; //디폴트 RValue 복사생성자

public:
	void GH_SetCutIndex(size_t _Index, GH_RENDERPIVOT _Pivot = GH_RENDERPIVOT::CENTER);

	void GH_Render();

private:
	std::map<std::string, GH_FrameAnimation*> AllAnimation_;
	GH_FrameAnimation* CurrentAni_;

private:
	GH_FrameAnimation* GH_FindAnimation(const std::string& _name);

public:
	void GH_CreateAnimation(
		const std::string& _Name, //애니메이션의 이름
		const std::string& _ImageName, //들어갈 그림의 이름
		int _Start, //시작할 프레임의 번호
		int _End, //끝낼 프레임의 번호
		bool _Loop = true, //애니메이션 반복여부, 기본적으로는 true이나 false를 넣어주면 한번만 재생된다
		float _FrameTime = 0.1f, //그림과 그림이 바뀌는 시간의 간격, 기본은 0.1f초
		GH_RENDERPIVOT pivot_ = GH_RENDERPIVOT::CENTER //애니메이션의 중심점이 될 점의 위치, 기본은 가운데
	);

	void GH_ChangeAnimation(const std::string& _Name, bool _forceChange = false);
	//애니메이션 재생중에 강제로 바꾸면 true, 아니면 false가 기본
	void GH_AnimationUpdate();

	bool GH_IsCurAnimationName(const std::string& _Name);
	bool GH_IsCurAnimationEnd();

	int GH_GetCurAnimationFrame();
};

