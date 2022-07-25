#include "BossBettyJumpAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../BossBettyDataComponent.h"

CBossBettyJumpAttackNode::CBossBettyJumpAttackNode()
{
	SetTypeID(typeid(CBossBettyJumpAttackNode).hash_code());
}

CBossBettyJumpAttackNode::CBossBettyJumpAttackNode(const CBossBettyJumpAttackNode& Node)
{
}

CBossBettyJumpAttackNode::~CBossBettyJumpAttackNode()
{
}

void CBossBettyJumpAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// PunchDouble
	std::string AnimName = "PunchDouble";

	// >> Start
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);

	AnimInst->AddNotify(AnimName, "OnZMove", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableMoveZ);
	AnimInst->AddNotify(AnimName, "SlowMoveSpeed", 0,
		Data, &CBossBettyDataComponent::OnBossBettyApplyOutOfMapSurroundingColliderMoveSpeed);

	// >> 중간
	AnimInst->AddNotify(AnimName, "SlowMoveSpeed", 6,
		this, &CBossBettyJumpAttackNode::OnBossBettySlowMoveSpeed);
	AnimInst->AddNotify(AnimName, "OnEnableSpinCollider", 10,
		Data, &CBossBettyDataComponent::OnBossBettyEnableSpinCollider);
	AnimInst->AddNotify(AnimName, "DisableLookPlayer", 10,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
	AnimInst->AddNotify(AnimName, "DisableZMove", 25,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);
	AnimInst->AddNotify(AnimName, "CameraShake", 25,
		Data, &CBossBettyDataComponent::OnBossBettyNormalShakeCamera);

	// >> End
	// PunchDown Animation 이 끝나면 해당 위치에 2단 공격
	AnimInst->AddNotify(AnimName, "AttackDown", 35,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);
	AnimInst->AddNotify(AnimName, "EnableAttackCollider", 29,
		Data, &CBossBettyDataComponent::OnBossBettyEnableAttackCollider);
	AnimInst->AddNotify(AnimName, "DisableAttackCollider", 31,
		Data, &CBossBettyDataComponent::OnBossBettyDisableAttackCollider);
	AnimInst->AddNotify(AnimName, "AttackFarAttackCnt", 35,
		Data, &CBossBettyDataComponent::IncFarAttackCount);
	AnimInst->AddNotify(AnimName, "OnDisableSpinCollider", 35,
		Data, &CBossBettyDataComponent::OnBossBettyDisableSpinCollider);
	AnimInst->AddNotify(AnimName, "ResetMoveSpeed", 35,
		Data, &CBossBettyDataComponent::OnBossBettyResetOriginalMoveSpeed);

	AnimInst->SetEndFunction(AnimName,
		Data, &CBossBettyDataComponent::OnBossBettySetCurrentNodeNullPtr);

}

NodeResult CBossBettyJumpAttackNode::OnStart(float DeltaTime)
{
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	AnimInst->ChangeAnimation("PunchDouble");

	m_Owner->SetCurrentNode(this);

	return NodeResult::Node_True;
}

NodeResult CBossBettyJumpAttackNode::OnUpdate(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	return NodeResult::Node_True;
}

NodeResult CBossBettyJumpAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

void CBossBettyJumpAttackNode::OnBossBettySlowMoveSpeed()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	
	Data->SetCurMoveSpeed(Data->GetOriginMoveSpeed() * 0.7f);
}
