#include "GH_GameEngineDirectory.h"
#include <Windows.h>
#include <io.h>

GH_GameEngineDirectory::GH_GameEngineDirectory()
{
	//지금 내 주소를 string으로 알아오는 함수
	path_.resize(256);
	GetCurrentDirectoryA(static_cast<DWORD>(path_.size()), &path_[0]);

	for (size_t i = 0; i < path_.size(); i++)
	{
		path_[i] = '\0';
	}

	GetCurrentDirectoryA(static_cast<DWORD>(path_.size()), &path_[0]);
	size_t Zerocount = path_.find('\0');
	path_ = path_.substr(0, Zerocount);
	//substr => string에서 넣어준 두개의 값들 사이의 string만을 잘라냄
	path_ += "\\";
}

GH_GameEngineDirectory::~GH_GameEngineDirectory()
{
}

GH_GameEngineDirectory::GH_GameEngineDirectory(GH_GameEngineDirectory&& _Other) noexcept
{
}

GH_GameEngineDirectory::GH_GameEngineDirectory(const GH_GameEngineDirectory& _Other)
	: GH_GameEnginePath(_Other)
{

}

std::string GH_GameEngineDirectory::GH_DirectoryName()
{

	// reverseFind
	size_t Count = path_.rfind("\\");
	//rfind=>string을 오른쪽에서 왼쪽으로 움직이면서 ()안의 문자열을 찾아주는 함수
	//find와 반대방향으로 검색
	if (std::string::npos == Count)
	{
		return path_;
	}

	size_t StartCount = path_.rfind("\\", Count - 1);

	std::string Name = path_.substr(StartCount, Count);
	Name = Name.substr(1, Name.size() - 2);
	return Name;


}

bool GH_GameEngineDirectory::GH_IsRoot()
{
	int Count = 0;

	for (size_t i = 0; i < path_.size(); i++)
	{
		if (path_[i] == '\\')
		{
			++Count;
		}
	}

	if (1 >= Count)
	{
		return true;
	}

	return false;
	//들아온 주소가 c:\같은 하드디스크 제일 밖의 주소인지 확인
}

void GH_GameEngineDirectory::GH_MoveParent()
{
	size_t Count = path_.rfind("\\");
	if (std::string::npos == Count)
	{
		return;
	}

	size_t StartCount = path_.rfind("\\", Count - 1);
	path_ = path_.substr(0, StartCount + 1);
}

bool GH_GameEngineDirectory::GH_MoveParent(std::string _DirName)
{
	GH_GameEngineDirectory CheckDir = GH_GameEngineDirectory(*this);

	while (true)
	{
		if (_DirName == CheckDir.GH_DirectoryName())
		{
			break;
			//내 디렉터리가 정해준 디렉터리와 같아지면 브레이크
		}

		CheckDir.GH_MoveParent();
		//내 디렉터리를 상위로 한단계씩 올린다

		if (true == CheckDir.GH_IsRoot())
		{
			return false;
			//c:\나 d:\같은 맨 처음 디렉터리에 도착하면
			//찾는 디렉터리가 없다는 것이므로 false를 리턴
		}

	}

	this->path_ = CheckDir.path_;
	return true;
	//디렉터리를 바꿔주고 리턴
}

bool GH_GameEngineDirectory::GH_IsExist()
{
	if (0 == _access(path_.c_str(), 0))
	{
		return true;

	}

	return false;
}

bool GH_GameEngineDirectory::GH_MoveChild(std::string _DirName)
{
	//디렉터리를 추가로 붙히고, 그 뒤에 \\를 붙혀주는 함수
	//만약 들어온 디렉터리(string)의 앞과 뒤에 \\가 붙어있는 기형적인 형태일 경우,
	//그 \\를 둘다 제거하고 맨 뒤에만 \\를 붙혀줌

	if (0 == _DirName.size())
	{
		return false;
	}

	if (_DirName[0] == '\\')
	{
		_DirName = _DirName.substr(1, _DirName.size());
	}

	if (_DirName[_DirName.size() - 1] == '\\')
	{
		_DirName = _DirName.substr(0, _DirName.size() - 1);
	}

	GH_GameEngineDirectory NewDir;
	NewDir.path_ = this->path_ + _DirName + "\\";

	if (false == NewDir.GH_IsExist())
	{
		return false;
		//내가 만들어낸 주소가 만약 실제 하드에 존재하지 않는다면, false
	}

	this->path_ += _DirName + "\\";
	return true;

}

std::string GH_GameEngineDirectory::GH_CreateFileNametoPath(std::string _FileName)
{
	return path_ + _FileName;
}