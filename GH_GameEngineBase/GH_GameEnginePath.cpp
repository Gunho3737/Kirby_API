#include "GH_GameEnginePath.h"

GH_GameEnginePath::GH_GameEnginePath()
	: path_("")
{

}

GH_GameEnginePath::~GH_GameEnginePath()
{
}

GH_GameEnginePath::GH_GameEnginePath(GH_GameEnginePath&& _Other) noexcept
{
}

GH_GameEnginePath::GH_GameEnginePath(const GH_GameEnginePath& _Other)
	: path_(_Other.path_)
{
}