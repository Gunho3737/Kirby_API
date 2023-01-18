#include "GH_Renderer.h"
#include <GH_GameEngineDebug.h>
#include <GH_GameEngineTime.h>
#include "GH_ResourcesManager.h"
#include "GH_GameEngineImage.h"
#include "GH_Actor.h"

GH_Renderer::GH_FrameAnimation::GH_FrameAnimation()
	: Start_(-1),
	End_(-1),
	Current_(-1),
	FrameTime_(0.0f),
	CurFrameTime_(0.0f),
	Pivot_(GH_RENDERPIVOT::BOT),
	Renderer_(nullptr),
	Loop_(nullptr),
	Image_(nullptr)
{
}

void GH_Renderer::GH_FrameAnimation::GH_Update()
{
	IsEnd_ = false;
	Renderer_->GH_SetCutIndex(Current_, Pivot_);
	CurFrameTime_ -= GH_GameEngineTime::GH_GetInst().GH_GetDeltaTime();
	if (0.0f >= CurFrameTime_)
	{
		CurFrameTime_ = FrameTime_;
		++Current_;

		//프레임이 증가해서 애니메이션의 마지막을 넘어가면
		if (End_ < Current_)
		{
			IsEnd_ = true;

			if (true == Loop_)
			{
				//루프가 true이면 현재상태를 start로 돌린다
				Current_ = Start_;
			}
			else
			{
				--Current_;
			}
		}
	}

	Renderer_->GH_Render();
}


GH_Renderer::GH_Renderer()
	: ImagePivot_(GH_float4::ZERO),
	Pivotpos_(GH_float4::ZERO),
	Rendersize_(GH_float4::ZERO),
	Imagepos_(GH_float4::ZERO),
	ImageSize_(GH_float4::ZERO),
	IsCamEffect_(true)
{

}

GH_Renderer::~GH_Renderer()
{
	std::map<std::string, GH_FrameAnimation*>::iterator AniStart = AllAnimation_.begin();
	std::map<std::string, GH_FrameAnimation*>::iterator AniEnd = AllAnimation_.end();

	for (; AniStart != AniEnd; ++AniStart)
	{
		if (nullptr == AniStart->second)
		{
			continue;
		}

		delete AniStart->second;
		AniStart->second = nullptr;
	}

	AllAnimation_.clear();
}

GH_Renderer::GH_Renderer(GH_Renderer&& _Other) noexcept
{
}

void GH_Renderer::GH_SetImage(const std::string& _Name)
{
	Image_ = GH_ResourcesManager::GH_GetInst().GH_FindGameImage(_Name);
	//String 이름으로 이미지 찾기

	if (nullptr == Image_)
	{
		GH_GameEngineDebug::Assert();
		return;
		//_Name이 없으면 터트림
	}

	Imagepos_ = GH_float4::ZERO;
	//들여올 이미지는 0,0 좌표부터 들여옴
	//이미지 자체를 편집하고/자르기 때문에

	ImageSize_ = Image_->GH_GetSize();
	Rendersize_ = Image_->GH_GetSize();

	ImagePivot_ = Rendersize_.GH_halffloat4(); //이미지의 중심을 중앙에 뒀다
}

void GH_Renderer::GH_Render()
{
	GH_GameEngineImage* BackImage = GH_ResourcesManager::GH_GetInst().GH_GetBackBufferImage();
	//BackBuffer를 받아옴

	GH_float4 ActorPos = Actor_->GH_GetCamEffectPos();

	if (false == IsCamEffect_)
	{
		ActorPos = Actor_->GH_GetPos();
	}

	GH_float4 RenderPos = ActorPos + Pivotpos_ - ImagePivot_;
	//렌더를 할땐 0,0에서 중심점으로 이동시킴+Pivotpos_값을 더하고 빼서 위치를 조정해줌

	//TransparentBlt을 이용해서 이미지를 출력한다
	BackImage->GH_TransCopy(Image_,
		RenderPos,
		Rendersize_,
		Imagepos_,
		ImageSize_,
		RGB(255, 0, 255) 
	);

	//애니메이션의 작동원리
	//1. cut 함수를 이용해서 애니메이션으로 만들 부분의 이미지의 시작점과 크기를 지정
	//2. TransparentBlt 함수가 출력할 이미지 크기와 시작좌표를 지정한다
	//3. 이미지를 출력하고, 내가 지정해준 시간이 지나면 TransparentBlt에 넣어줄 이미지를 출력할 시작좌표를 바꾼다
	//4. 이미지를 출력시키는 시작좌표가 바뀌엇으므로, 다음 프레임의 이미지로 바뀌게 된다.

}

void GH_Renderer::GH_SetCutIndex(size_t _Index, GH_RENDERPIVOT _Pivot)
{
	if (false == Image_->GH_IsCut())
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	Imagepos_ = Image_->GH_GetCutPos(_Index);
	ImageSize_ = Image_->GH_GetCutSize(_Index);
	Rendersize_ = ImageSize_;

	ImagePivot_ = Rendersize_.GH_halffloat4();

	switch (_Pivot)
	{
	case GH_RENDERPIVOT::CENTER:
		ImagePivot_ = Rendersize_.GH_halffloat4();
		break;
	case GH_RENDERPIVOT::BOT:
		ImagePivot_ = Rendersize_.GH_halffloat4();
		ImagePivot_.y += Rendersize_.GH_halffloat4().y;
		break;
	case GH_RENDERPIVOT::LEFTTOP:
		ImagePivot_ = GH_float4::ZERO;
		break;
	default:
		GH_GameEngineDebug::Assert();
		break;
	}
}

GH_Renderer::GH_FrameAnimation* GH_Renderer::GH_FindAnimation(const std::string& _name)
{
	std::map<std::string, GH_FrameAnimation*>::iterator FindAniIter = AllAnimation_.find(_name);

	if (FindAniIter == AllAnimation_.end())
	{
		return nullptr;
	}

	return FindAniIter->second;
}

void GH_Renderer::GH_ChangeAnimation(const std::string& _Name, bool _forceChange)
{
	if (nullptr != CurrentAni_ &&
		false == _forceChange &&
		CurrentAni_->GH_GetName() == _Name)
	{
		return;
	}

	CurrentAni_ = GH_FindAnimation(_Name);
	CurrentAni_->GH_Reset();

	if (nullptr == CurrentAni_)
	{
		GH_GameEngineDebug::Assert();
		return;
	}
}

void GH_Renderer::GH_CreateAnimation(const std::string& _Name,
	const std::string& _ImageName,
	int _Start,
	int _End,
	bool _Loop,
	float _FrameTime,
	GH_RENDERPIVOT pivot_)
{
	if (nullptr != GH_FindAnimation(_Name))
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	GH_GameEngineImage* ImagePtr = GH_ResourcesManager::GH_GetInst().GH_FindGameImage(_ImageName);

	if (nullptr == ImagePtr)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	if (false == ImagePtr->GH_IsCut())
	{
		GH_GameEngineDebug::MessageBoxError("자르지 않은 이미지를 넣으려고 합니다");
		return;
	}
	GH_FrameAnimation* NewAnimation = new GH_FrameAnimation();

	NewAnimation->GH_SetName(_Name);
	NewAnimation->Start_ = _Start;//첫 프레임 이미지 설정
	NewAnimation->End_ = _End; //마지막 프레임 이미지 설정
	NewAnimation->FrameTime_ = _FrameTime; //한프레임당 움직이는 시간 설정
	NewAnimation->Image_ = ImagePtr; //이미지 이름으로 찾은 이미지를 이용
	NewAnimation->Current_ = _Start; //현재 프레임을 첫 프레임으로 만듬
	NewAnimation->CurFrameTime_ = _FrameTime;
	NewAnimation->Pivot_ = pivot_; //중심점의 위치를 정함
	NewAnimation->Renderer_ = this;
	NewAnimation->Loop_ = _Loop;

	AllAnimation_.insert(std::map<std::string, GH_FrameAnimation*>::value_type(_Name, NewAnimation));
	//만든 애니메이션을 map에 저장

}

void GH_Renderer::GH_AnimationUpdate()
{
	if (nullptr == CurrentAni_)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	CurrentAni_->GH_Update();

}

bool GH_Renderer::GH_IsCurAnimationName(const std::string& _Name)
{
	if (nullptr == CurrentAni_)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	if (CurrentAni_->GH_GetName() == _Name)
	{
		return true;
	}

	return false;
}

bool GH_Renderer::GH_IsCurAnimationEnd()
{
	if (nullptr == CurrentAni_)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	return CurrentAni_->IsEnd_;
}

int GH_Renderer::GH_GetCurAnimationFrame()
{
	if (nullptr == CurrentAni_)
	{
		GH_GameEngineDebug::Assert();
		return false;
	}

	return CurrentAni_->Current_;

}