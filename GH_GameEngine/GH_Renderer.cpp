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

		//�������� �����ؼ� �ִϸ��̼��� �������� �Ѿ��
		if (End_ < Current_)
		{
			IsEnd_ = true;

			if (true == Loop_)
			{
				//������ true�̸� ������¸� start�� ������
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
	//String �̸����� �̹��� ã��

	if (nullptr == Image_)
	{
		GH_GameEngineDebug::Assert();
		return;
		//_Name�� ������ ��Ʈ��
	}

	Imagepos_ = GH_float4::ZERO;
	//�鿩�� �̹����� 0,0 ��ǥ���� �鿩��
	//�̹��� ��ü�� �����ϰ�/�ڸ��� ������

	ImageSize_ = Image_->GH_GetSize();
	Rendersize_ = Image_->GH_GetSize();

	ImagePivot_ = Rendersize_.GH_halffloat4(); //�̹����� �߽��� �߾ӿ� �״�
}

void GH_Renderer::GH_Render()
{
	GH_GameEngineImage* BackImage = GH_ResourcesManager::GH_GetInst().GH_GetBackBufferImage();
	//BackBuffer�� �޾ƿ�

	GH_float4 ActorPos = Actor_->GH_GetCamEffectPos();

	if (false == IsCamEffect_)
	{
		ActorPos = Actor_->GH_GetPos();
	}

	GH_float4 RenderPos = ActorPos + Pivotpos_ - ImagePivot_;
	//������ �Ҷ� 0,0���� �߽������� �̵���Ŵ+Pivotpos_���� ���ϰ� ���� ��ġ�� ��������

	//TransparentBlt�� �̿��ؼ� �̹����� ����Ѵ�
	BackImage->GH_TransCopy(Image_,
		RenderPos,
		Rendersize_,
		Imagepos_,
		ImageSize_,
		RGB(255, 0, 255) 
	);

	//�ִϸ��̼��� �۵�����
	//1. cut �Լ��� �̿��ؼ� �ִϸ��̼����� ���� �κ��� �̹����� �������� ũ�⸦ ����
	//2. TransparentBlt �Լ��� ����� �̹��� ũ��� ������ǥ�� �����Ѵ�
	//3. �̹����� ����ϰ�, ���� �������� �ð��� ������ TransparentBlt�� �־��� �̹����� ����� ������ǥ�� �ٲ۴�
	//4. �̹����� ��½�Ű�� ������ǥ�� �ٲ�����Ƿ�, ���� �������� �̹����� �ٲ�� �ȴ�.

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
		GH_GameEngineDebug::MessageBoxError("�ڸ��� ���� �̹����� �������� �մϴ�");
		return;
	}
	GH_FrameAnimation* NewAnimation = new GH_FrameAnimation();

	NewAnimation->GH_SetName(_Name);
	NewAnimation->Start_ = _Start;//ù ������ �̹��� ����
	NewAnimation->End_ = _End; //������ ������ �̹��� ����
	NewAnimation->FrameTime_ = _FrameTime; //�������Ӵ� �����̴� �ð� ����
	NewAnimation->Image_ = ImagePtr; //�̹��� �̸����� ã�� �̹����� �̿�
	NewAnimation->Current_ = _Start; //���� �������� ù ���������� ����
	NewAnimation->CurFrameTime_ = _FrameTime;
	NewAnimation->Pivot_ = pivot_; //�߽����� ��ġ�� ����
	NewAnimation->Renderer_ = this;
	NewAnimation->Loop_ = _Loop;

	AllAnimation_.insert(std::map<std::string, GH_FrameAnimation*>::value_type(_Name, NewAnimation));
	//���� �ִϸ��̼��� map�� ����

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