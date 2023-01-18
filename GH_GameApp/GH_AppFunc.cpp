#include "GH_AppFunc.h"
#include <GH_LevelManager.h>
#include <GH_GameEngineTime.h>
#include <GH_ResourcesManager.h>
#include <GH_GameEngineWindow.h>
#include <GH_GameEngineInput.h>
#include <GH_GameEngineSound.h>
#include <GH_GameEngineDirectory.h>
#include <GH_GameEngineImage.h>

void GH_AppResourcesInit()
{
	//사용할 리소스를 여기에 전부 불러온다
	


	GH_GameEngineDirectory Dir = GH_GameEngineDirectory();
	Dir.GH_MoveParent("Kirby_API");
	//내 프로젝트와 내 리소스의 공통폴더로 moveParent

	if (false == Dir.GH_MoveChild("\\Resources\\"))
	{
		//리소스들의 공통폴더까지 MoveChild로 이동
		//만약 없는 폴더면 터트림
		GH_GameEngineDebug::Assert();
		return;
	}

	{ 
		//이미지

		//StartLevel

		//이미지의 주소를 상대경로로 받아온다
		GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Title_Screen.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Background\\Title_Screen.bmp"));
		//GH_CreateFileNametoPath로 경로 지정



		//State_One_One

		//UI렌더
		{
			GH_GameEngineImage* HPImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("HPBar.bmp",
				Dir.GH_CreateFileNametoPath("Image\\UI\\HPBar.bmp"));
			HPImage->GH_Cut({ 200, 200 });

			GH_GameEngineImage* AbilityImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Kirby_Ability_Icon.bmp",
				Dir.GH_CreateFileNametoPath("Image\\UI\\Kirby_Ability_Icon.bmp"));
			AbilityImage->GH_Cut({ 240, 240 });

			GH_GameEngineImage* BossHPImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Boss_HPBar.bmp",
				Dir.GH_CreateFileNametoPath("Image\\UI\\Boss_HPBar.bmp"));
			BossHPImage->GH_Cut({ 400, 400 });

			GH_GameEngineImage* PressEnterImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Press_Enter.bmp",
				Dir.GH_CreateFileNametoPath("Image\\UI\\Press_Enter.bmp"));
			PressEnterImage->GH_Cut({ 500, 500 });
		}

		//캐릭터 렌더
		{
		GH_GameEngineImage* KirbyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("normal_kirby.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Kirby\\normal_kirby.bmp"));
		KirbyImage->GH_Cut({ 200, 200 });

		GH_GameEngineImage* DanceKirbyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("KirbyDance_one.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Kirby\\KirbyDance_one.bmp"));
		DanceKirbyImage->GH_Cut({ 1100, 800 });

		DanceKirbyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("KirbyDance_two.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Kirby\\KirbyDance_two.bmp"));
		DanceKirbyImage->GH_Cut({ 1100, 800 });

		DanceKirbyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("KirbyDance_three.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Kirby\\KirbyDance_three.bmp"));
		DanceKirbyImage->GH_Cut({ 1100, 800 });

		GH_GameEngineImage* WeakEnemyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Waddle_Di.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Enemy\\Waddle_Di.bmp"));
		WeakEnemyImage->GH_Cut({ 180, 180 });

		GH_GameEngineImage* BeamEnemyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Beam_Enemy.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Enemy\\Beam_Enemy.bmp"));
		BeamEnemyImage->GH_Cut({ 500, 500 });

		GH_GameEngineImage* CutterEnemyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Cutter_Enemy.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Enemy\\Cutter_Enemy.bmp"));
		CutterEnemyImage->GH_Cut({ 200, 200 });

		GH_GameEngineImage* TreeEnemyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Tree_Boss.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Enemy\\Tree_Boss.bmp"));
		TreeEnemyImage->GH_Cut({ 500, 500 });

		GH_GameEngineImage* BeamKirbyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("beam_kirby.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Kirby\\beam_kirby.bmp"));
		BeamKirbyImage->GH_Cut({ 300, 300 });

		GH_GameEngineImage* CutterKirbyImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("cutter_kirby.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Kirby\\cutter_kirby.bmp"));
		CutterKirbyImage->GH_Cut({ 250, 250 });

		}
	
		//총알, 타격/피격 이미지
		{
			GH_GameEngineImage* AppleImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Apple.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Enemy\\Apple.bmp"));
			AppleImage->GH_Cut({ 200, 200 });


			GH_GameEngineImage* BreathImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Breath.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Enemy\\Breath.bmp"));
			BreathImage->GH_Cut({ 100, 100 });

			GH_GameEngineImage* DrainEffectImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Kirby_Drain.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Kirby_Drain.bmp"));
			DrainEffectImage->GH_Cut({ 400, 400 });

			GH_GameEngineImage* StarImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Star.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Star.bmp"));
			StarImage->GH_Cut({ 180, 180 });

			GH_GameEngineImage* KirbyBreathImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Kirby_Breath.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Kirby_Breath.bmp"));
			KirbyBreathImage->GH_Cut({ 150, 150 });

			GH_GameEngineImage* MonsterDeathImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Monster_Death.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Monster_Death.bmp"));
			MonsterDeathImage->GH_Cut({ 400, 400 });

			GH_GameEngineImage* StarImpactImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Star_Impact.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Star_Impact.bmp"));
			StarImpactImage->GH_Cut({ 250, 250 });


			GH_GameEngineImage* EatEffectImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Eat_Effect.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Eat_Effect.bmp"));
			EatEffectImage->GH_Cut({ 200, 200 });

			GH_GameEngineImage* TreeDamageEffectImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Tree_Damage_Effect.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Tree_Damage_Effect.bmp"));
			TreeDamageEffectImage->GH_Cut({ 400, 400 });

			GH_GameEngineImage* CutterAttImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Cutter_Att.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Cutter_Att.bmp"));
			CutterAttImage->GH_Cut({ 200, 200 });

			GH_GameEngineImage* BeamAttImage = GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Beam_Att.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Effect\\Beam_Att.bmp"));
			BeamAttImage->GH_Cut({ 500, 500 });

		}

		
		//맵, 충돌맵
		{
			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("1-1_big.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\1-1_big.bmp"));

			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Bit_1-1.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\Bit_1-1.bmp"));

			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("1-1_background.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\1-1_background.bmp"));


			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("1-2.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\1-2.bmp"));

			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Bit_1-2.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\Bit_1-2.bmp"));

			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("1-2_background.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\1-2_background.bmp"));

			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Boss_BackGround.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\Boss_BackGround.bmp"));

			GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Bit_Boss.bmp",
				Dir.GH_CreateFileNametoPath("Image\\Background\\Bit_Boss.bmp"));
		}
		//EndLevel
		GH_ResourcesManager::GH_GetInst().GH_LoadGameImage("Complete_.bmp",
			Dir.GH_CreateFileNametoPath("Image\\Background\\Complete_.bmp"));
	}

	{
		//사운드


		//StartLevel
		GH_GameEngineSound::GH_GetInst().GH_LoadSound("Title.mp3",
			Dir.GH_CreateFileNametoPath("Sound\\BackGroundMusic\\Title.mp3"));


		//State_One_One
		GH_GameEngineSound::GH_GetInst().GH_LoadSound("Green_Greens.mp3"
		, Dir.GH_CreateFileNametoPath("Sound\\BackGroundMusic\\Green_Greens.mp3"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("Boss_Fight.mp3"
			, Dir.GH_CreateFileNametoPath("Sound\\BackGroundMusic\\Boss_Fight.mp3"));

		
		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyDrainChange.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyDrainChange.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyDamage.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyDamage.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyDrain.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyDrain.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyDrain_Repeat.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyDrain_Repeat.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("AppleDrop.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\AppleDrop.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("BeamMonsterAttack.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\BeamMonsterAttack.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("BossDead.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\BossDead.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("BossYawn.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\BossYawn.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("CutterMonsterAttack.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\CutterMonsterAttack.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyBeamAttack.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyBeamAttack.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyCutterAttack.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyCutterAttack.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyDoorIn.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyDoorIn.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyDrainEat.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyDrainEat.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyFly.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyFly.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyJump.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyJump.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyStarShot.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyStarShot.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyYawnAttack.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\KirbyYawnAttack.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("MonsterDead.wav",
			Dir.GH_CreateFileNametoPath("Sound\\SoundEffect\\MonsterDead.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("Die.wav",
			Dir.GH_CreateFileNametoPath("Sound\\BackGroundMusic\\Die.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("Ending.wav",
			Dir.GH_CreateFileNametoPath("Sound\\BackGroundMusic\\Ending.wav"));

		GH_GameEngineSound::GH_GetInst().GH_LoadSound("KirbyDance.wav",
			Dir.GH_CreateFileNametoPath("Sound\\BackGroundMusic\\KirbyDance.wav"));

	}
}

void GH_AppRelease()
{
	//메모리 정리
	//순서를 바꾸지 말것
	GH_LevelManager::GH_Destory();
	GH_ResourcesManager::GH_Destory();

	GH_GameEngineSound::GH_Destroy();
	GH_GameEngineInput::GH_Destory();
	GH_GameEngineWindow::GH_Destory();
	GH_GameEngineTime::GH_Destory();
	



}