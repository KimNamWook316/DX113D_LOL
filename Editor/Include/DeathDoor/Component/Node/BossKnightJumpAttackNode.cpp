#include "BossKnightJumpAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../KnightDataComponent.h"

CBossKnightJumpAttackNode::CBossKnightJumpAttackNode()
{
	SetTypeID(typeid(CBossKnightJumpAttackNode).hash_code());
}

CBossKnightJumpAttackNode::CBossKnightJumpAttackNode(const CBossKnightJumpAttackNode& Node)	:
	CActionNode(Node)
{
}

CBossKnightJumpAttackNode::~CBossKnightJumpAttackNode()
{
}

void CBossKnightJumpAttackNode::Init()
{
	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string AnimName = "DashSlam";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->AddNotify(AnimName, "IncreaseMeleeCount", 0, Data, &CKnightDataComponent::IncreaseMeleeAttackCount);
	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 11, Data, &CKnightDataComponent::OnLookPlayerMove);
	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnAttackJump", 12, 19, this, &CBossKnightJumpAttackNode::OnAttackJumpMove);
	AnimInst->AddNotify(AnimName, "OnHitBoxActive", 20, Data, &CKnightDataComponent::OnActiveMeleeAttackCollider);
	AnimInst->AddNotify(AnimName, "OnHitBoxDisable", 20, Data, &CKnightDataComponent::OnInActiveMeleeAttackCollider);
	AnimInst->SetEndFunction(AnimName, Data, &CKnightDataComponent::OnEndAnimJudgeContinueAttack);

	m_MoveSpeed = Data->GetMoveSpeed();
}

NodeResult CBossKnightJumpAttackNode::OnStart(float DeltaTime)
{
	std::string AnimName = "DashSlam";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation(AnimName);

	return NodeResult::Node_True;
}

NodeResult CBossKnightJumpAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossKnightJumpAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

// 직선 이동
void CBossKnightJumpAttackNode::OnAttackJumpMove(float DeltaTime)
{
	CGameObject* MyObj = m_Owner->GetOwner()->GetGameObject();

	// Walk Speed 두배로 이동
	MyObj->AddWorldPosByLocalAxis(AXIS::AXIS_Z, m_MoveSpeed * 2.f);
}
