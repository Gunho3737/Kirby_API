#include "InGame_UI.h"
#include <GH_GameEngineImage.h>
#include <GH_ResourcesManager.h>
#include <GH_GameEngineCollision.h>
#include <GH_GameEngineInput.h>
#include <GH_Renderer.h>
#include <GH_GameEngineWindow.h>
#include <GH_LevelManager.h>
#include <GH_GameEngineInput.h>
#include "One_One_Back.h"
#include <GH_GameEngineTime.h>
#include "Normal_Kirby.h"
#include <GH_Level.h>
#include "Stage_one_one.h"
#include "TreeBoss.h"


InGame_UI::InGame_UI()
{
	GH_SetRenderOrder(10);

}

InGame_UI::~InGame_UI()
{
}

InGame_UI::InGame_UI(InGame_UI&& _Other) noexcept
{
}

void InGame_UI::GH_Start()
{
	{
		HPBarRender_ = GH_CreateRenderer("HPBar.bmp");
		HPBarRender_->GH_CreateAnimation("HP6", "HPBar.bmp", 0, 0, false);
		HPBarRender_->GH_CreateAnimation("HP5", "HPBar.bmp", 1, 1, false);
		HPBarRender_->GH_CreateAnimation("HP4", "HPBar.bmp", 2, 2, false);
		HPBarRender_->GH_CreateAnimation("HP3", "HPBar.bmp", 3, 3, false);
		HPBarRender_->GH_CreateAnimation("HP2", "HPBar.bmp", 4, 4, false);
		HPBarRender_->GH_CreateAnimation("HP1", "HPBar.bmp", 5, 5, false);
		HPBarRender_->GH_CreateAnimation("HP0", "HPBar.bmp", 6, 6, false);
		HPBarRender_->GH_SetPivotPos({72, 650});
	}

	{
		KirbyAbilityRender_ = GH_CreateRenderer("Kirby_Ability_Icon.bmp");
		KirbyAbilityRender_->GH_CreateAnimation("Ability_UI_Normal", "Kirby_Ability_Icon.bmp",0, 0, false);
		KirbyAbilityRender_->GH_CreateAnimation("Ability_UI_Beam", "Kirby_Ability_Icon.bmp", 1, 1, false);
		KirbyAbilityRender_->GH_CreateAnimation("Ability_UI_Cutter", "Kirby_Ability_Icon.bmp", 2, 2, false);
		KirbyAbilityRender_->GH_SetPivotPos({70,760});
	}

	{
		BossHPBarRender_ = GH_CreateRenderer("Boss_HPBar.bmp");
		BossHPBarRender_->GH_CreateAnimation("BossHP9", "Boss_HPBar.bmp", 0, 0, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP8", "Boss_HPBar.bmp", 1, 1, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP7", "Boss_HPBar.bmp", 2, 2, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP6", "Boss_HPBar.bmp", 3, 3, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP5", "Boss_HPBar.bmp", 4, 4, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP4", "Boss_HPBar.bmp", 5, 5, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP3", "Boss_HPBar.bmp", 6, 6, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP2", "Boss_HPBar.bmp", 7, 7, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP1", "Boss_HPBar.bmp", 8, 8, false);
		BossHPBarRender_->GH_CreateAnimation("BossHP0", "Boss_HPBar.bmp", 9, 9, false);
		BossHPBarRender_->GH_SetPivotPos({ 845,765 });
	}
}

void InGame_UI::GH_Update()
{ 
	//UI의 위치는 카메라의 위치와 맞춘다
	GH_SetPos(GH_GetLevel()->GH_GetCamPos());
	
	
}

void InGame_UI::GH_Render()
{
	
	switch (Stage_one_one::NormalKirbyPtr->AbilityType_)
	{
	case GH_KirbyAbilityState::Normal:
		KirbyAbilityRender_->GH_ChangeAnimation("Ability_UI_Normal");
		break;
	case GH_KirbyAbilityState::Beam:
		KirbyAbilityRender_->GH_ChangeAnimation("Ability_UI_Beam");
		break;
	case GH_KirbyAbilityState::Cutter:
		KirbyAbilityRender_->GH_ChangeAnimation("Ability_UI_Cutter");
		break;
	default:
		break;
	}

	switch (Stage_one_one::NormalKirbyPtr->HP)
	{
	case 6:
		HPBarRender_->GH_ChangeAnimation("HP6");
		break;
	case 5:
		HPBarRender_->GH_ChangeAnimation("HP5");
		break;
	case 4:
		HPBarRender_->GH_ChangeAnimation("HP4");
		break;
	case 3:
		HPBarRender_->GH_ChangeAnimation("HP3");
		break;
	case 2:
		HPBarRender_->GH_ChangeAnimation("HP2");
		break;
	case 1:
		HPBarRender_->GH_ChangeAnimation("HP1");
		break;
	case 0:
		HPBarRender_->GH_ChangeAnimation("HP0");
		break;
	default:
		break;
	}


	if (Stage_one_one::TreeBossPtr != nullptr)
	{
		switch (Stage_one_one::TreeBossPtr->HP)
		{
		case 9:
			BossHPBarRender_->GH_ChangeAnimation("BossHP9");
			break;
		case 8:
			BossHPBarRender_->GH_ChangeAnimation("BossHP8");
			break;
		case 7:
			BossHPBarRender_->GH_ChangeAnimation("BossHP7");
			break;
		case 6:
			BossHPBarRender_->GH_ChangeAnimation("BossHP6");
			break;
		case 5:
			BossHPBarRender_->GH_ChangeAnimation("BossHP5");
			break;
		case 4:
			BossHPBarRender_->GH_ChangeAnimation("BossHP4");
			break;
		case 3:
			BossHPBarRender_->GH_ChangeAnimation("BossHP3");
			break;
		case 2:
			BossHPBarRender_->GH_ChangeAnimation("BossHP2");
			break;
		case 1:
			BossHPBarRender_->GH_ChangeAnimation("BossHP1");
			break;
		case 0:
			BossHPBarRender_->GH_ChangeAnimation("BossHP0");
			break;
		default:
			break;
		}
		BossHPBarRender_->GH_AnimationUpdate();
	}

	KirbyAbilityRender_->GH_AnimationUpdate();
	HPBarRender_->GH_AnimationUpdate();


	
	
}