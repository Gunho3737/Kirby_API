#pragma once
#include <map>
#include <string>
#include <GH_GameEngineMath.h>
#include <GH_GameEngineNameBase.h>
//�з� : ������
//�뵵 : �׸��� �׸��µ� �ʿ��� ���Ǳ�� ����
//���� : ���Ͱ� ����/�������� �ʿ��� ����� �����Ѵ�

// ����� ���� �ڷ���
// ���� ����(�ϳ��� ��ü�� ��� ���¸� �����ٰ� �Ҷ�)�� 
// ��������� ǥ���ϰ� ������ ���� ���
enum class GH_RENDERPIVOT
{	//switch���� 0,1,2,3 ������ �ϱ⺸��
	//��������� �˱� ����ܾ���� �̿�
	//string���� switch�� �ϴ°� ��� �̿��ϴ°�
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

private: //����Ʈ ���� ������
	friend GH_Actor;

private:
	GH_Actor* Actor_;
	GH_float4 ImagePivot_; //�̹����� �߽��� �� ��ǥ=> ���⿡ ���� �߿� �δ�, �߾ӿ� �δ�, 0,0�� �δ� ����
	GH_float4 Pivotpos_; //�⺻ �߽������� ���� ��ġ�� �ٲٰ� ������ �����ִ� ���� ������ ����
	GH_float4 Rendersize_; //������ ������ ��ǥ
	GH_float4 Imagepos_; //�鿩�� �̹����� ������
	GH_float4 ImageSize_;//�̹����� ũ��
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
	GH_Renderer& operator=(const GH_Renderer& _Other) = delete; //����Ʈ ���Կ�����
	GH_Renderer& operator=(GH_Renderer&& _Other) = delete; //����Ʈ RValue ���Կ�����
public:
	GH_Renderer(); //����Ʈ ������
	~GH_Renderer(); //����Ʈ �Ҹ���
	GH_Renderer(const GH_Renderer& _Other) = delete; //����Ʈ ���� ������
	GH_Renderer(GH_Renderer&& _Other) noexcept; //����Ʈ RValue ���������

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
		const std::string& _Name, //�ִϸ��̼��� �̸�
		const std::string& _ImageName, //�� �׸��� �̸�
		int _Start, //������ �������� ��ȣ
		int _End, //���� �������� ��ȣ
		bool _Loop = true, //�ִϸ��̼� �ݺ�����, �⺻�����δ� true�̳� false�� �־��ָ� �ѹ��� ����ȴ�
		float _FrameTime = 0.1f, //�׸��� �׸��� �ٲ�� �ð��� ����, �⺻�� 0.1f��
		GH_RENDERPIVOT pivot_ = GH_RENDERPIVOT::CENTER //�ִϸ��̼��� �߽����� �� ���� ��ġ, �⺻�� ���
	);

	void GH_ChangeAnimation(const std::string& _Name, bool _forceChange = false);
	//�ִϸ��̼� ����߿� ������ �ٲٸ� true, �ƴϸ� false�� �⺻
	void GH_AnimationUpdate();

	bool GH_IsCurAnimationName(const std::string& _Name);
	bool GH_IsCurAnimationEnd();

	int GH_GetCurAnimationFrame();
};

