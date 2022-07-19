#pragma once

enum class DDActionNode
{
	Move,
	Idle,
	NormalAttack,
	Death,
	RotateAttackDirection,
	ReadyToShoot,
	ShootNode,
	CancleShootNode,
	AddFallingFloorCallback,
	Lockstone3TriggerBoxAction,
	FindPath,
	BossKnightContinueAttack,
	BossKnightFinalAttack,
	BossKnightJumpAttack,
	BossKnightMeleeAttack,
	BossKnightSlamEnd,
	Max
};

enum class DDConditionNode
{
	MoveInputCheckNode,
	MouseLButtonCheckNode,
	NoInterruptNode,
	AttackTargetCheck,
	MouseRButtonCheckNode,
	MouseRButtonUpCheckNode,
	Lockstone3TriggerBoxHitCheck,
	CheckDetectRange,
	MeleeAttackRangeCheck,
	PostAttackDelayCheck,
	BossKnightFinalAttackCheck,
	BossKnightJumpAttackRangeCheck,
	Max
};

enum class DDSceneComponentType
{
	PlayerNormalAttackCheckCollider,
	EyeLaserComponent,
	PlayerHookComponent,
	MonsterPathFindCollider,
	Max
};

enum class DDObjectComponentType
{
	ObjectDataComponent,
	PlayerDataComponent,
	GameStateComponent,
	MonsterData,
	KnightData,
	Max
};
