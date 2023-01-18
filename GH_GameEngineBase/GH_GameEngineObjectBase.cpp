#include "GH_GameEngineObjectBase.h"

GH_GameEngineObjectBase::GH_GameEngineObjectBase()
	: IsDeath_(false),
	IsUpdate_(true),
	parent_(nullptr)
{

}

GH_GameEngineObjectBase::~GH_GameEngineObjectBase()
{
}

GH_GameEngineObjectBase::GH_GameEngineObjectBase(GH_GameEngineObjectBase&& _Other) noexcept
{
}