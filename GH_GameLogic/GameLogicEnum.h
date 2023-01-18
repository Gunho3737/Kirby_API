#pragma once

enum class GAMECOLLISIONGROUP
{
	//충돌체를 만들때 내가 몇번 그룹인지 명시적으로 나타냄
	MAP,
	PLAYER,
	MONSTER,
	BULLET,
	MONSTERATTACK
};

enum class DEBUGMODE
{
	PLAYMODE,
	DEBUGMODE

};

enum class GH_MonsterDirectionState
{
	LEFT,
	RIGHT,
};

enum class GH_MonsterState
{
	Walk,
	Damage,
	Dead,
	GetDrain,
	Attack,
	Jump,
	AttackReady,
	Idle,
	Breath
};

enum class GH_KirbyAbilityState
{
	Normal,
	DrainAfter,
	Beam,
	Cutter,


};

//플레잉스테이지 상황을 저장하는 enum
enum class StageState
{
	ONEONE,
	ONETWO,
	BOSS

};
