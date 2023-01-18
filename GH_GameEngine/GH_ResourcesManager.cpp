#include "GH_ResourcesManager.h"
#include "GH_GameEngineImage.h"
#include <GH_GameEngineDebug.h>
#include "GH_GameEngineWindow.h"

GH_ResourcesManager* GH_ResourcesManager::Instance = new GH_ResourcesManager();

GH_ResourcesManager::GH_ResourcesManager()
{


}

GH_ResourcesManager::~GH_ResourcesManager()
{
	std::map<std::string, GH_GameEngineImage*>::iterator StartIter = GlobalManagedImages_.begin();
	std::map<std::string, GH_GameEngineImage*>::iterator EndIter = GlobalManagedImages_.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}
	GlobalManagedImages_.clear();

	if (nullptr != BackBufferImage_)
	{
		delete BackBufferImage_;
		BackBufferImage_ = nullptr;
	}

	if (nullptr != WindowImage_)
	{
		delete WindowImage_;
		WindowImage_ = nullptr;
	}
}

GH_ResourcesManager::GH_ResourcesManager(GH_ResourcesManager&& _Other) noexcept
{
}

GH_GameEngineImage* GH_ResourcesManager::GH_FindGameImage(std::string _name)
{
	std::map<std::string, GH_GameEngineImage*>::iterator FindIter
		= GlobalManagedImages_.find(_name);

	if (FindIter == GlobalManagedImages_.end())
	{
		return nullptr;
	}


	return FindIter->second;

	//string���� ��üã��
}

GH_GameEngineImage* GH_ResourcesManager::GH_LoadGameImage(std::string _name, std::string _path)
{
	//find�� ���� �����̸���
	//���߿� ����Ҷ� ����� ��θ� string���� �޾Ƽ�
	//map���� �����ϴ� �Լ�


	if (nullptr != GH_FindGameImage(_name))
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
		//���� �ѹ� ������ ������ �ٽ� �������� �Ѵٸ� ��Ʈ��
	}

	GH_GameEngineImage* NewImage = new GH_GameEngineImage();
	if (false == NewImage->GH_Load(_path))
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
		//���� ��ο� �ҷ��� ������ ���ٸ� ��Ʈ��
	}

	GlobalManagedImages_.insert(std::map<std::string, GH_GameEngineImage*>::value_type(_name, NewImage));
	return NewImage;

}


GH_GameEngineImage* GH_ResourcesManager::GH_GetBackBufferImage()
{
	return BackBufferImage_;
}

void GH_ResourcesManager::GH_InitializeWindowImage(HDC _windowbackgroundDC)
{
	// ó���� ������â�� DC�� �޾Ƽ� �̹����� �ϳ��� �����,
	// �������� �Ҷ�, �������� �̹������� �� ���� â���ٰ� �������ִ°�
	// ��� �����ָ鼭 ������ �ϴ°� �ƴ϶�
	// 
	// 1. ���� ������â�� ������ �����ؼ� ���θ��� �̹��������Ͷ� ����� ���ο� DC�� ����
	// 2. �� �̹��� �����Ϳ� �������� �̹������� ���θ��� �̹��� �����Ϳ� ������
	// 3. ���� ���� �̹��� �����͸� �Ѳ����� ���� ������â�� ����


	if (nullptr == _windowbackgroundDC)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	BackBufferImage_ = new GH_GameEngineImage();
	BackBufferImage_->GH_Create(_windowbackgroundDC, GH_GameEngineWindow::GH_GetInst().GH_GetSize());
	//�� ������â�� �Ȱ��� ����� ���Ӱ� ������� bmp�̹��� �����͸� ������

	WindowImage_ = new GH_GameEngineImage();
	WindowImage_->GH_Create(_windowbackgroundDC);
	//�� ������â�� �����͸� �޾ƿ�

}

void GH_ResourcesManager::DoubleBuffering()
{
	WindowImage_->GH_BitCopy(BackBufferImage_, GH_float4::ZERO, GH_GameEngineWindow::GH_GetInst().GH_GetSize());
	//BackBuffer�� ����� �̹��� ������ ��¥ ������ â�� �Űܿ´�
}