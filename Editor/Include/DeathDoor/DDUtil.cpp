#include "DDUtil.h"

// TODO : DeathDoor Component 추가 시마다 업데이트
#include "Component\EyeLaserComponent.h"
#include "Component\KnightDataComponent.h"
#include "Component\BossBettyDataComponent.h"
#include "Component\MonsterDataComponent.h"
#include "Component\MonsterPathFindCollider.h"
#include "Component\ObjectDataComponent.h"
#include "Component\GameStateComponent.h"
#include "Component\PlayerDataComponent.h"
#include "Component\PlayerHookComponent.h"
#include "Component\PlayerNormalAttackCheckCollider.h"
#include "Component\LurkerDataComponent.h"
#include "Component\MonsterNavAgent.h"
#include "Component\PlayerBowComponent.h"
#include "Component\ProjectileComponent.h"
#include "Component\ArrowComponent.h"
#include "Component\CrowBossDataComponent.h"
#include "Component\TinyCrowDataComponent.h"

std::string CDDUtil::DDConditionNodeTypeToString(DDConditionNode NodeType)
{
	switch (NodeType)
	{
	case DDConditionNode::MoveInputCheckNode:
		return "MoveInputCheckNode";

	case DDConditionNode::MouseLButtonCheckNode:
		return "MouseLButtonCheckNode";

	case DDConditionNode::NoInterruptNode:
		return "NoInterruptNode";

	case DDConditionNode::AttackTargetCheck:
		return "AttackTargetCheck";

	case DDConditionNode::MouseRButtonCheckNode:
		return "MouseRButtonCheckNode";

	case DDConditionNode::MouseRButtonUpCheckNode:
		return "MouseRButtonUpCheckNode";

	case DDConditionNode::Lockstone3TriggerBoxHitCheck:
		return "Lockstone3TriggerBoxHitCheck";

	case DDConditionNode::CheckDetectRange:
		return "CheckDetectRange";

	case DDConditionNode::PlayerEnterZoneCheck:
		return "PlayerEnterZoneCheck";
	case DDConditionNode::PlayerEnterTriggerIsEnable:
		return "PlayerEnterTriggerIsEnable";

	case DDConditionNode::MeleeAttackRangeCheck:
		return "MeleeAttackRangeCheck";

	case DDConditionNode::PostAttackDelayCheck:
		return "PostAttackDelayCheck";

	case DDConditionNode::AttackCoolTimeCheck:
		return "AttackCoolTimeCheck";

	case DDConditionNode::BossKnightFinalAttackCheck:
		return "BossKnightFinalAttackCheck";

	case DDConditionNode::BossKnightJumpAttackRangeCheck:
		return "BossKnightJumpAttackRangeCheck";

	case DDConditionNode::BossKnightCutScenePlayCheck:
		return "BossKnightCutScenePlayCheck";

	case DDConditionNode::BossKnightContinueAttackCheck:
		return "BossKnightContinueAttackCheck";

	case DDConditionNode::BossKnightPlayerEnterZoneCheck:
		return "BossKnightPlayerEnterZoneCheck";

	// Boss Betty
	case DDConditionNode::BossBettyCheckAttackDir:
		return "BossBettyCheckAttackDir";

	case DDConditionNode::BossBettyCheckFarAttackType:
		return "BossBettyCheckFarAttackType";

	case DDConditionNode::BossBettyCheckHPState:
		return "BossBettyCheckHPState";

	case DDConditionNode::BossBettyCheckThrowAttack:
		return "BossBettyCheckThrowAttack";

	// Crow Boss
	case DDConditionNode::CrowBossCutScenePlayCheck:
		return "CrowBossCutScenePlayCheck";
	case DDConditionNode::CrowBossPhasePickNode:
		return "CrowBossPhasePickNode";
	case DDConditionNode::CrowBossBypassCheck:
		return "CrowBossBypassCheck";
	case DDConditionNode::CrowBossJumpStartCheck:
		return "CrowBossJumpStartCheck";
	case DDConditionNode::CrowBossLandingCheck:
		return "CrowBossLandingCheck";
	case DDConditionNode::CrowBossJumpAttackRangeCheck:
		return "CrowBossJumpAttackRangeCheck";
	case DDConditionNode::CrowBossDoneCheck:
		return "CrowBossDoneCheck";
	case DDConditionNode::CrowBossFirstShootCheck:
		return "CrowBossFirstShootCheck";
	case DDConditionNode::CrowBossShootCheck:
		return "CrowBossShootCheck";
	case DDConditionNode::CrowBossShootEndCheck:
		return "CrowBossShootEndCheck";
	case DDConditionNode::CrowBossShootReadyCheck:
		return "CrowBossShootReadyCheck";


	case DDConditionNode::HPCheck:
		return "HPCheck";

	case DDConditionNode::DeathCheck:
		return "DeathCheck";

	case DDConditionNode::IsCombatCheck:
		return "IsCombatCheck";

	case DDConditionNode::PathFindEnableCheck:
		return "PathFindEnableCheck";

	case DDConditionNode::HitCheckNode:
		return "HitCheckNode";

	case DDConditionNode::RollInputCheck:
		return "RollInputCheck";

	case DDConditionNode::UpdateInputQueue:
		return "UpdateInputQueue";
	}

	return "";
}

DDConditionNode CDDUtil::StringToDDConditionNodeType(const std::string& Str)
{
	if (Str == "BossKnightFinalAttackCheck")
	{
		return DDConditionNode::BossKnightFinalAttackCheck;
	}
	else if (Str == "MoveInputCheckNode")
	{
		return DDConditionNode::MoveInputCheckNode;
	}
	else if (Str == "MouseLButtonCheckNode")
	{
		return DDConditionNode::MouseLButtonCheckNode;
	}
	else if (Str == "NoInterruptNode")
	{
		return DDConditionNode::NoInterruptNode;
	}
	else if (Str == "AttackTargetCheck")
	{
		return DDConditionNode::AttackTargetCheck;
	}
	else if (Str == "MouseRButtonCheckNode")
	{
		return DDConditionNode::MouseRButtonCheckNode;
	}
	else if (Str == "MouseRButtonUpCheckNode")
	{
		return DDConditionNode::MouseRButtonUpCheckNode;
	}
	else if (Str == "Lockstone3TriggerBoxHitCheck")
	{
		return DDConditionNode::Lockstone3TriggerBoxHitCheck;
	}
	else if (Str == "CheckDetectRange")
	{
		return DDConditionNode::CheckDetectRange;
	}
	else if (Str == "PlayerEnterZoneCheck")
	{
		return DDConditionNode::PlayerEnterZoneCheck;
	}
	else if (Str == "PlayerEnterTriggerIsEnable")
	{
		return DDConditionNode::PlayerEnterTriggerIsEnable;
	}
	else if (Str == "BossKnightJumpAttackRangeCheck")
	{
		return DDConditionNode::BossKnightJumpAttackRangeCheck;
	}
	else if (Str == "MeleeAttackRangeCheck")
	{
		return DDConditionNode::MeleeAttackRangeCheck;
	}
	else if (Str == "PostAttackDelayCheck")
	{
		return DDConditionNode::PostAttackDelayCheck;
	}
	else if (Str == "AttackCoolTimeCheck")
	{
		return DDConditionNode::AttackCoolTimeCheck;
	}
	
	else if (Str == "BossKnightCutScenePlayCheck")
	{
		return DDConditionNode::BossKnightCutScenePlayCheck;
	}
	else if (Str == "BossKnightPlayerEnterZoneCheck")
	{
		return DDConditionNode::BossKnightPlayerEnterZoneCheck;
	}
	else if (Str == "BossKnightContinueAttackCheck")
	{
		return DDConditionNode::BossKnightContinueAttackCheck;
	}
	// Boss Betty
	else if (Str == "BossBettyCheckAttackDir")
	{
		return DDConditionNode::BossBettyCheckAttackDir;
	}
	else if (Str == "BossBettyCheckFarAttackType")
	{
		return DDConditionNode::BossBettyCheckFarAttackType;
	}
	else if (Str == "BossBettyCheckHPState")
	{
		return DDConditionNode::BossBettyCheckHPState;
	}
	else if (Str == "BossBettyCheckThrowAttack")
	{
		return DDConditionNode::BossBettyCheckThrowAttack;
	}
	// Crow Boss
	else if (Str == "CrowBossCutScenePlayCheck")
	{
		return DDConditionNode::CrowBossCutScenePlayCheck;
	}
	else if (Str == "CrowBossPhasePickNode")
	{
		return DDConditionNode::CrowBossPhasePickNode;
	}
	else if (Str == "CrowBossJumpStartCheck")
	{
		return DDConditionNode::CrowBossJumpStartCheck;
	}
	else if (Str == "CrowBossLandingCheck")
	{
		return DDConditionNode::CrowBossLandingCheck;
	}
	else if (Str == "CrowBossJumpAttackRangeCheck")
	{
	return DDConditionNode::CrowBossJumpAttackRangeCheck;
	}

	else if (Str == "HPCheck")
	{
		return DDConditionNode::HPCheck;
	}
	else if (Str == "DeathCheck")
	{
		return DDConditionNode::DeathCheck;
	}
	else if (Str == "IsCombatCheck")
	{
		return DDConditionNode::IsCombatCheck;
	}
	else if (Str == "PathFindEnableCheck")
	{
		return DDConditionNode::PathFindEnableCheck;
	}
	else if (Str == "HitCheckNode")
	{
		return DDConditionNode::HitCheckNode;
	}
	else if (Str == "RollInputCheck")
	{
		return DDConditionNode::RollInputCheck;
	}
	else if (Str == "UpdateInputQueue")
	{
	return DDConditionNode::UpdateInputQueue;
	}

	return DDConditionNode(-1);
}

std::string CDDUtil::DDActionNodeTypeToString(DDActionNode NodeType)
{
	switch (NodeType)
	{
	case DDActionNode::Move:
		return "Move";

	case DDActionNode::Idle:
		return "Idle";

	case DDActionNode::NormalAttack:
		return "NormalAttack";

	case DDActionNode::Death:
		return "Death";

	case DDActionNode::RotateAttackDirection:
		return "RotateAttackDirection";

	case DDActionNode::ReadyToShoot:
		return "ReadyToShoot";

	case DDActionNode::ShootNode:
		return "ShootNode";

	case DDActionNode::CancleShootNode:
		return "CancleShootNode";

	case DDActionNode::AddFallingFloorCallback:
		return "AddFallingFloorCallback";

	case DDActionNode::Lockstone3TriggerBoxAction:
		return "Lockstone3TriggerBoxAction";

	case DDActionNode::FindPath:
		return "FindPath";

	case DDActionNode::BossKnightContinueAttack:
		return "BossKnightContinueAttack";

	case DDActionNode::BossKnightFinalAttack:
		return "BossKnightFinalAttack";

	case DDActionNode::BossKnightJumpAttack:
		return "BossKnightJumpAttack";

	case DDActionNode::BossKnightMeleeAttack:
		return "BossKnightMeleeAttack";

	case DDActionNode::BossKnightSlamEnd:
		return "BossKnightSlamEnd";

	case DDActionNode::BossKnightIdle:
		return "BossKnightIdle";

	case DDActionNode::BossKnightWalk:
		return "BossKnightWalk";

	case DDActionNode::BossKnightCutScenePlay:
		return "BossKnightCutScenePlay";

	// Boss Betty
	case DDActionNode::BossBettyAngryAttack:
		return "BossBettyAngryAttack";
	case DDActionNode::BossBettyChangeAttackDir:
		return "BossBettyChangeAttackDir";
	case DDActionNode::BossBettyCloseAttack:
		return "BossBettyCloseAttack";
	case DDActionNode::BossBettyJumpAttack:
		return "BossBettyJumpAttack";
	case DDActionNode::BossBettySpinAttack:
		return "BossBettySpinAttack";
	case DDActionNode::BossBettyThrowAttack:
		return "BossBettyThrowAttack";

	// Crow Boss
	case DDActionNode::CrowBossCutScenePlayNode:
		return "CrowBossCutScenePlayNode";
	case DDActionNode::CrowBossRunNode:
		return "CrowBossRunNode";
	case DDActionNode::CrowBossSpinNode:
		return "CrowBossSpinNode";
	case DDActionNode::CrowBossJump:
		return "CrowBossJump";
	case DDActionNode::CrowBossFly:
		return "CrowBossFlyNode";
	case DDActionNode::CrowBossMoveShootSpot:
		return "CrowBossMoveShootSpot";
	case DDActionNode::CrowBossResetShootState:
		return "CrowBossResetShootState";
	case DDActionNode::CrowBossShoot:
		return "CrowBossShoot";
	case DDActionNode::CrowBossSlide:
		return "CrowBossSlide";
	case DDActionNode::CrowBossSpitting:
		return "CrowBossSpitting";


	case DDActionNode::ClearPathList:
		return "ClearPathList";

	case DDActionNode::HitBack:
		return "HitBack";

	case DDActionNode::PlayerRoll:
		return "PlayerRoll";
	}

	return "";
}

DDActionNode CDDUtil::StringToDDActionNodeType(const std::string& Str)
{
	if (Str == "BossKnightFinalAttack")
	{
		return DDActionNode::BossKnightFinalAttack;
	}
	else if (Str == "Move")
	{
		return DDActionNode::Move;
	}
	else if (Str == "Idle")
	{
		return DDActionNode::Idle;
	}
	else if (Str == "NormalAttack")
	{
		return DDActionNode::NormalAttack;
	}
	else if (Str == "Death")
	{
		return DDActionNode::Death;
	}
	else if (Str == "RotateAttackDirection")
	{
		return DDActionNode::RotateAttackDirection;
	}
	else if (Str == "ReadyToShoot")
	{
		return DDActionNode::ReadyToShoot;
	}
	else if (Str == "ShootNode")
	{
		return DDActionNode::ShootNode;
	}
	else if (Str == "CancleShootNode")
	{
		return DDActionNode::CancleShootNode;
	}
	else if (Str == "AddFallingFloorCallback")
	{
		return DDActionNode::AddFallingFloorCallback;
	}
	else if (Str == "Lockstone3TriggerBoxAction")
	{
		return DDActionNode::Lockstone3TriggerBoxAction;
	}
	else if (Str == "FindPath")
	{
		return DDActionNode::FindPath;
	}
	else if (Str == "BossKnightContinueAttack")
	{
		return DDActionNode::BossKnightContinueAttack;
	}
	else if (Str == "BossKnightJumpAttack")
	{
		return DDActionNode::BossKnightJumpAttack;
	}
	else if (Str == "BossKnightMeleeAttack")
	{
		return DDActionNode::BossKnightMeleeAttack;
	}
	else if (Str == "BossKnightSlamEnd")
	{
		return DDActionNode::BossKnightSlamEnd;
	}
	else if (Str == "BossKnightWalk")
	{
		return DDActionNode::BossKnightWalk;
	}
	else if (Str == "BossKnightCutScenePlay")
	{
		return DDActionNode::BossKnightCutScenePlay;
	}

	// Boss Betty
	else if (Str == "BossBettyAngryAttack")
	{
		return DDActionNode::BossBettyAngryAttack;
	}
	else if (Str == "BossBettyChangeAttackDir")
	{
		return DDActionNode::BossBettyChangeAttackDir;
	}
	else if (Str == "BossBettyCloseAttack")
	{
		return DDActionNode::BossBettyCloseAttack;
	}
	else if (Str == "BossBettyJumpAttack")
	{
		return DDActionNode::BossBettyJumpAttack;
	}
	else if (Str == "BossBettyThrowAttack")
	{
		return DDActionNode::BossBettyThrowAttack;
	}

	// Crow Boss
	else if (Str == "CrowBossCutScenePlayNode")
	{
		return DDActionNode::CrowBossCutScenePlayNode;
	}
	else if (Str == "CrowBossRunNode")
	{
		return DDActionNode::CrowBossRunNode;
	}
	else if (Str == "CrowBossSpinNode")
	{
		return DDActionNode::CrowBossSpinNode;
	}
	else if (Str == "CrowBossJump")
	{
		return DDActionNode::CrowBossJump;
	}
	else if (Str == "CrowBossFly")
	{
		return DDActionNode::CrowBossFly;
	}
	else if (Str == "CrowBossMoveShootSpot")
	{
		return DDActionNode::CrowBossMoveShootSpot;
	}
	else if (Str == "CrowBossResetShootState")
	{
		return DDActionNode::CrowBossResetShootState;
	}
	else if (Str == "CrowBossShoot")
	{
		return DDActionNode::CrowBossShoot;
	}
	else if (Str == "CrowBossSlide")
	{
		return DDActionNode::CrowBossSlide;
	}
	else if (Str == "CrowBossSpitting")
	{
	return DDActionNode::CrowBossSpitting;
	}


	else if (Str == "ClearPathList")
	{
		return DDActionNode::ClearPathList;
	}
	else if (Str == "HitBack")
	{
		return DDActionNode::HitBack;
	}
	else if (Str == "PlayerRoll")
	{
		return DDActionNode::PlayerRoll;
	}

	return DDActionNode(-1);
}

std::string CDDUtil::DDSceneComponentTypeToString(DDSceneComponentType Type)
{
	switch (Type)
	{
	case DDSceneComponentType::PlayerNormalAttackCheckCollider:
		return "PlayerNormalAttackCheckCollider";
	case DDSceneComponentType::EyeLaserComponent:
		return "EyeLaserComponent";
	case DDSceneComponentType::PlayerHookComponent:
		return "PlayerHookComponent";
	case DDSceneComponentType::PlayerBowComponent:
		return "PlayerBowComponent";
	}

	return "";
}

DDSceneComponentType CDDUtil::StringToDDSceneComponentType(const std::string& Str)
{
	if (Str == "PlayerNormalAttackCheckCollider")
	{
		return DDSceneComponentType::PlayerNormalAttackCheckCollider;
	}
	else if (Str == "EyeLaserComponent")
	{
		return DDSceneComponentType::EyeLaserComponent;
	}
	else if (Str == "PlayerHookComponent")
	{
		return DDSceneComponentType::PlayerHookComponent;
	}
	//else if (Str == "MonsterPathFindCollider")
	//{
	//	return DDSceneComponentType::MonsterPathFindCollider;
	//}
	return DDSceneComponentType(-1);
}

size_t CDDUtil::DDSceneComponentTypeToTypeID(DDSceneComponentType Type)
{
	switch (Type)
	{
	case DDSceneComponentType::PlayerNormalAttackCheckCollider:
		return typeid(CPlayerNormalAttackCheckCollider).hash_code();
	case DDSceneComponentType::EyeLaserComponent:
		return typeid(CEyeLaserComponent).hash_code();
	case DDSceneComponentType::PlayerHookComponent:
		return typeid(CPlayerHookComponent).hash_code();
	case DDSceneComponentType::PlayerBowComponent:
		return typeid(CPlayerBowComponent).hash_code();
	}
	return -1;
}

std::string CDDUtil::DDObjectComponentTypeToString(DDObjectComponentType Type)
{
	switch (Type)
	{
	case DDObjectComponentType::PlayerDataComponent:
		return "PlayerDataComponent";

	case DDObjectComponentType::ObjectDataComponent:
		return "ObjectDataComponent";

	case DDObjectComponentType::GameStateComponent:
		return "GameStateComponent";

	case DDObjectComponentType::MonsterData:
		return "MonsterData";

	case DDObjectComponentType::KnightData:
		return "KnightData";
		
	case DDObjectComponentType::BossBettyData:
		return "BossBettyData";

	case DDObjectComponentType::CrowBossData:
		return "CrowBossData";

	case DDObjectComponentType::LurkerData:
		return "LurkerData";

	case DDObjectComponentType::MonsterNavAgent:
		return "MonsterNavAgent";

	case DDObjectComponentType::ProjectileComponent:
		return "ProjectileComponent";

	case DDObjectComponentType::ArrowComponent:
		return "ArrowComponent";

	case DDObjectComponentType::TinyCrowDataComponent:
		return "TinyCrowDataComponent";
	}

	return "";
}

DDObjectComponentType CDDUtil::StringToDDObjectComponentType(const std::string& Str)
{
	if (Str == "MonsterData")
	{
		return DDObjectComponentType::MonsterData;
	}
	else if (Str == "ObjectDataComponent")
	{
		return DDObjectComponentType::ObjectDataComponent;
	}
	else if (Str == "PlayerDataComponent")
	{
		return DDObjectComponentType::PlayerDataComponent;
	}
	else if (Str == "GameStateComponent")
	{
		return DDObjectComponentType::GameStateComponent;
	}
	else if (Str == "KnightData")
	{
		return DDObjectComponentType::KnightData;
	}
	else if (Str == "BossBettyData")
	{
		return DDObjectComponentType::BossBettyData;
	}
	else if (Str == "CrowBossData")
	{
		return DDObjectComponentType::CrowBossData;
	}
	else if (Str == "LurkerData")
	{
		return DDObjectComponentType::LurkerData;
	}
	else if (Str == "MonsterNavAgent")
	{
		return DDObjectComponentType::MonsterNavAgent;
	}
	else if (Str == "ProjectileComponent")
	{
		return DDObjectComponentType::ProjectileComponent;
	}
	else if (Str == "ArrowComponent")
	{
		return DDObjectComponentType::ArrowComponent;
	}

	return DDObjectComponentType(-1);
}

size_t CDDUtil::DDObjectComponentTypeToTypeID(DDObjectComponentType Type)
{
	switch (Type)
	{
	case DDObjectComponentType::GameStateComponent:
		return typeid(CGameStateComponent).hash_code();
	case DDObjectComponentType::KnightData:
		return typeid(CKnightDataComponent).hash_code();
	case DDObjectComponentType::BossBettyData:
		return typeid(CBossBettyDataComponent).hash_code();
	case DDObjectComponentType::CrowBossData:
		return typeid(CCrowBossDataComponent).hash_code();
	case DDObjectComponentType::MonsterData:
		return typeid(CMonsterDataComponent).hash_code();
	case DDObjectComponentType::ObjectDataComponent:
		return typeid(CObjectDataComponent).hash_code();
	case DDObjectComponentType::PlayerDataComponent:
		return typeid(CPlayerDataComponent).hash_code();
	case DDObjectComponentType::LurkerData:
		return typeid(CLurkerDataComponent).hash_code();
	case DDObjectComponentType::MonsterNavAgent:
		return typeid(CMonsterNavAgent).hash_code();
	case DDObjectComponentType::ProjectileComponent:
		return typeid(CProjectileComponent).hash_code();
	case DDObjectComponentType::ArrowComponent:
		return typeid(CArrowComponent).hash_code();
	case DDObjectComponentType::TinyCrowDataComponent:
		return typeid(CTinyCrowDataComponent).hash_code();
	}
	return -1;
}

Vector4 CDDUtil::LerpColor(const Vector4& ColorStart, const Vector4& ColorEnd, float ElapsedTime, float MaxTime)
{
	float Ratio = ElapsedTime / MaxTime;

	return (ColorStart * (1.f - Ratio) + ColorEnd * Ratio);
}
