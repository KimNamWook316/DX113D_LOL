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
	BossKnightIdle,
	BossKnightMeleeAttack,
	BossKnightSlamEnd,
	BossKnightWalk,
	BossKnightCutScenePlay,
	ClearPathList,
	HitBack,
	PlayerRoll,
	// BossBetty
	BossBettyAngryAttack,
	BossBettyChangeAttackDir,
	BossBettyCloseAttack,
	BossBettyJumpAttack,
	BossBettySpinAttack,
	BossBettyThrowAttack,
	// CrowBoss
	CrowBossCutScenePlayNode,
	CrowBossRunNode,
	CrowBossSpinNode,
	CrowBossJump,
	CrowBossFly,
	CrowBossMoveShootSpot,
	CrowBossResetShootState,
	CrowBossShoot,
	CrowBossSlide,
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
	PlayerEnterZoneCheck,
	PlayerEnterTriggerIsEnable,
	MeleeAttackRangeCheck,
	PostAttackDelayCheck,
	BossKnightFinalAttackCheck,
	BossKnightJumpAttackRangeCheck,
	BossKnightCutScenePlayCheck,
	BossKnightPlayerEnterZoneCheck,
	BossKnightContinueAttackCheck,
	HPCheck,
	DeathCheck,
	IsCombatCheck,
	PathFindEnableCheck,
	HitCheckNode,
	RollInputCheck,
	UpdateInputQueue,
	// BossBetty
	BossBettyCheckAttackDir,
	BossBettyCheckFarAttackType,
	BossBettyCheckHPState,
	BossBettyCheckThrowAttack,
	// CrowBoss
	CrowBossCutScenePlayCheck,
	CrowBossPhasePickNode,
	CrowBossBypassCheck,
	CrowBossJumpStartCheck,
	CrowBossLandingCheck,
	CrowBossJumpAttackRangeCheck,
	CrowBossDoneCheck,
	CrowBossFirstShootCheck,
	CrowBossShootCheck,
	CrowBossShootEndCheck,
	CrowBossShootReadyCheck,
	Max
};

enum class DDSceneComponentType
{
	PlayerNormalAttackCheckCollider,
	EyeLaserComponent,
	PlayerHookComponent,
	PlayerBowComponent,
	Max
};

enum class DDObjectComponentType
{
	ObjectDataComponent,
	PlayerDataComponent,
	GameStateComponent,
	MonsterData,
	KnightData,
	BossBettyData,
	CrowBossData,
	LurkerData,
	MonsterNavAgent,
	ProjectileComponent,
	ArrowComponent,
	Max
};


enum class HookResult
{
	// 날아가고 있는중
	OnShoot,
	NoCollision,
	Collision,
	CollisionEnd
};

enum class CrowBossShootState
{
	Ready,
	ReadyEnd,
	Teleport,
	Shoot,
	ShootEnd,
	Fly,
	Done,
	Max
};