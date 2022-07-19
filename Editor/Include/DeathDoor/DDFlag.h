#pragma once

enum class DDActionNode
{
	BossKnightContinueAttack,
	BossKnightFinalAttack,
	BossKnightJumpAttack,
	BossKnightMeleeAttack,
	BossKnightSlamEnd,
	Max
};

enum class DDConditionNode
{
	MeleeAttackRangeCheck,
	PostAttackDelayCheck,
	BossKnightFinalAttackCheck,
	BossKnightJumpAttackRangeCheck,
	Max
};

enum class DDSceneComponentType
{
};

enum class DDObjectComponentType
{
	MonsterData,
	KnightData,
};
