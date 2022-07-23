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
	AnimInst->AddNotify(AnimName, "OnEnableSpinCollider", 0,
		Data, &CBossBettyDataComponent::OnBossBettyEnableSpinCollider);
	AnimInst->AddNotify(AnimName, "SlowMoveSpeed", 0,
		this, &CBossBettyJumpAttackNode::OnBossBettySlowMoveSpeed);

	// >> 중간
	AnimInst->AddNotify(AnimName, "DisableLookPlayer", 10,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnZMove", 5,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableMoveZ);
	AnimInst->AddNotify(AnimName, "DisableZMove", 24,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);
	AnimInst->AddNotify(AnimName, "ResetMoveSpeed", 26,
		Data, &CBossBettyDataComponent::OnBossBettyResetOriginalMoveSpeed);

	// >> End
	// PunchDown Animation 이 끝나면 해당 위치에 2단 공격
	AnimInst->AddNotify(AnimName, "AttackDown", 45,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);
	AnimInst->AddNotify(AnimName, "AttackFarAttackCnt", 45,
		Data, &CBossBettyDataComponent::IncFarAttackCount);
	AnimInst->AddNotify(AnimName, "OnDisableSpinCollider", 45,
		Data, &CBossBettyDataComponent::OnBossBettyDisableSpinCollider);

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

	Data->SetCurMoveSpeed(Data->GetOriginMoveSpeed() * 0.8f);
	// m_CurMoveSpeed = 0.f;
}
