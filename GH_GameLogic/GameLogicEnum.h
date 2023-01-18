#pragma once

enum class GAMECOLLISIONGROUP
{
	//�浹ü�� ���鶧 ���� ��� �׷����� ��������� ��Ÿ��
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

//�÷��׽������� ��Ȳ�� �����ϴ� enum
enum class StageState
{
	ONEONE,
	ONETWO,
	BOSS

};
