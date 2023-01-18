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

	//string으로 객체찾기
}

GH_GameEngineImage* GH_ResourcesManager::GH_LoadGameImage(std::string _name, std::string _path)
{
	//find을 위한 파일이름과
	//나중에 출력할때 사용할 경로를 string으로 받아서
	//map으로 저장하는 함수


	if (nullptr != GH_FindGameImage(_name))
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
		//만약 한번 가져온 파일을 다시 가져오려 한다면 터트림
	}

	GH_GameEngineImage* NewImage = new GH_GameEngineImage();
	if (false == NewImage->GH_Load(_path))
	{
		GH_GameEngineDebug::Assert();
		return nullptr;
		//만약 경로에 불러올 파일이 없다면 터트림
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
	// 처음에 윈도우창의 DC를 받아서 이미지를 하나를 만들고,
	// 렌더링을 할때, 차례차례 이미지들을 내 원래 창에다가 복사해주는걸
	// 모두 보여주면서 렌더링 하는게 아니라
	// 
	// 1. 현재 윈도우창의 정보를 복사해서 새로만든 이미지데이터랑 연결된 새로운 DC를 만듬
	// 2. 그 이미지 데이터에 복사해줄 이미지들을 새로만든 이미지 데이터에 복사함
	// 3. 새로 만든 이미지 데이터를 한꺼번에 원본 윈도우창에 복사


	if (nullptr == _windowbackgroundDC)
	{
		GH_GameEngineDebug::Assert();
		return;
	}

	BackBufferImage_ = new GH_GameEngineImage();
	BackBufferImage_->GH_Create(_windowbackgroundDC, GH_GameEngineWindow::GH_GetInst().GH_GetSize());
	//내 윈도우창과 똑같은 사양의 새롭게 만들어진 bmp이미지 데이터를 만들음

	WindowImage_ = new GH_GameEngineImage();
	WindowImage_->GH_Create(_windowbackgroundDC);
	//내 윈도우창의 데이터를 받아옴

}

void GH_ResourcesManager::DoubleBuffering()
{
	WindowImage_->GH_BitCopy(BackBufferImage_, GH_float4::ZERO, GH_GameEngineWindow::GH_GetInst().GH_GetSize());
	//BackBuffer에 복사된 이미지 정보를 진짜 윈도우 창에 옮겨온다
}