#include "BossKnightMeleeAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../KnightDataComponent.h"

CBossKnightMeleeAttackNode::CBossKnightMeleeAttackNode()
{
	SetTypeID(typeid(CBossKnightMeleeAttackNode).hash_code());
}

CBossKnightMeleeAttackNode::CBossKnightMeleeAttackNode(const CBossKnightMeleeAttackNode& Node)	:
	CActionNode(Node)
{
}

CBossKnightMeleeAttackNode::~CBossKnightMeleeAttackNode()
{
}

void CBossKnightMeleeAttackNode::Init()
{
	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string AnimName = "Slam";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTrace", 0, 4, (CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);
	AnimInst->AddNotify(AnimName, "HitBoxActive", 11 ,Data, &CKnightDataComponent::OnActiveMeleeAttackCollider);
	AnimInst->AddNotify(AnimName, "HitBoxActive", 15, Data, &CKnightDataComponent::OnActiveMeleeAttackCollider);
	AnimInst->SetEndFunction(AnimName, Data, &CKnightDataComponent::OnEndAnimJudgeContinueAttack);
}

NodeResult CBossKnightMeleeAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	std::string AnimName = "Slam";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation(AnimName);

	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->IncreaseMeleeAttackCount();

	return NodeResult::Node_True;
}

NodeResult CBossKnightMeleeAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossKnightMeleeAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}
