#include "GH_GameEngineDirectory.h"
#include <Windows.h>
#include <io.h>

GH_GameEngineDirectory::GH_GameEngineDirectory()
{
	//���� �� �ּҸ� string���� �˾ƿ��� �Լ�
	path_.resize(256);
	GetCurrentDirectoryA(static_cast<DWORD>(path_.size()), &path_[0]);

	for (size_t i = 0; i < path_.size(); i++)
	{
		path_[i] = '\0';
	}

	GetCurrentDirectoryA(static_cast<DWORD>(path_.size()), &path_[0]);
	size_t Zerocount = path_.find('\0');
	path_ = path_.substr(0, Zerocount);
	//substr => string���� �־��� �ΰ��� ���� ������ string���� �߶�
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
	//rfind=>string�� �����ʿ��� �������� �����̸鼭 ()���� ���ڿ��� ã���ִ� �Լ�
	//find�� �ݴ�������� �˻�
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
	//��ƿ� �ּҰ� c:\���� �ϵ��ũ ���� ���� �ּ����� Ȯ��
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
			//�� ���͸��� ������ ���͸��� �������� �극��ũ
		}

		CheckDir.GH_MoveParent();
		//�� ���͸��� ������ �Ѵܰ辿 �ø���

		if (true == CheckDir.GH_IsRoot())
		{
			return false;
			//c:\�� d:\���� �� ó�� ���͸��� �����ϸ�
			//ã�� ���͸��� ���ٴ� ���̹Ƿ� false�� ����
		}

	}

	this->path_ = CheckDir.path_;
	return true;
	//���͸��� �ٲ��ְ� ����
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
	//���͸��� �߰��� ������, �� �ڿ� \\�� �����ִ� �Լ�
	//���� ���� ���͸�(string)�� �հ� �ڿ� \\�� �پ��ִ� �������� ������ ���,
	//�� \\�� �Ѵ� �����ϰ� �� �ڿ��� \\�� ������

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
		//���� ���� �ּҰ� ���� ���� �ϵ忡 �������� �ʴ´ٸ�, false
	}

	this->path_ += _DirName + "\\";
	return true;

}

std::string GH_GameEngineDirectory::GH_CreateFileNametoPath(std::string _FileName)
{
	return path_ + _FileName;
}