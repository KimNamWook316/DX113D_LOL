#include "BossKnightContinueAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../KnightDataComponent.h"

CBossKnightContinueAttackNode::CBossKnightContinueAttackNode()
{
	SetTypeID(typeid(CBossKnightContinueAttackNode).hash_code());
}

CBossKnightContinueAttackNode::CBossKnightContinueAttackNode(const CBossKnightContinueAttackNode& Node)	:
	CActionNode(Node)
{
}

CBossKnightContinueAttackNode::~CBossKnightContinueAttackNode()
{
}

void CBossKnightContinueAttackNode::Init()
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 애니메이션 콜백 등록
	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	AnimInst->AddNotifyDeltaTimeFrameRange("SlamContinueFirst", "OnTrace", 9, 14, Data, &CKnightDataComponent::OnTracePlayerOnlyRotation);
 	AnimInst->SetEndFunction("SlamContinueFirst", (CMonsterDataComponent*)(Data), &CMonsterDataComponent::OnEndAnimPostAttackDelayOn);
 	AnimInst->AddNotify("SlamContinueFirst", "HitBoxActive", 21, Data, &CKnightDataComponent::OnActiveMeleeAttackCollider);
 	AnimInst->AddNotify("SlamContinueFirst", "HitBoxDisable", 25, Data, &CKnightDataComponent::OnInActiveMeleeAttackCollider);

	AnimInst->AddNotifyDeltaTimeFrameRange("Slam180CW", "OnTrace", 5, 10, Data, &CKnightDataComponent::OnTracePlayerOnlyRotation);
 	AnimInst->AddNotify("Slam180CW", "HitBoxActive", 21, Data, &CKnightDataComponent::OnActiveMeleeAttackCollider);
 	AnimInst->AddNotify("Slam180CW", "HitBoxDisable", 25, Data, &CKnightDataComponent::OnInActiveMeleeAttackCollider);
 	AnimInst->SetEndFunction("Slam180CW",(CMonsterDataComponent*)(Data), &CMonsterDataComponent::OnEndAnimPostAttackDelayOn);

	AnimInst->AddNotifyDeltaTimeFrameRange("Slam180CCW", "OnTrace", 5, 10, Data, &CKnightDataComponent::OnTracePlayerOnlyRotation);
 	AnimInst->AddNotify("Slam180CCW", "HitBoxActive", 21, Data, &CKnightDataComponent::OnActiveMeleeAttackCollider);
 	AnimInst->AddNotify("Slam180CCW", "HitBoxDisable", 25, Data, &CKnightDataComponent::OnInActiveMeleeAttackCollider);
 	AnimInst->SetEndFunction("Slam180CCW", (CMonsterDataComponent*)(Data), &CMonsterDataComponent::OnEndAnimPostAttackDelayOn);

	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	m_RotatePerSec = Data->GetRotateSpeed();
}

NodeResult CBossKnightContinueAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	// Melee Attack Count 증가
	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->IncreaseMeleeAttackCount();

	// 공격 회전 방향에 따라 다른 애니메이션 재생
	Knight_Attack_Rot_Type AttackType = Data->GetAttackRotationType();
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	switch (AttackType)
	{
	case Knight_Attack_Rot_Type::Front:
		AnimInst->ChangeAnimation("SlamContinueFirst");
		break;
	case Knight_Attack_Rot_Type::CW:
		AnimInst->ChangeAnimation("Slam180CW");
		break;
	case Knight_Attack_Rot_Type::CCW:
		AnimInst->ChangeAnimation("Slam180CCW");
		break;
	}

	return NodeResult();
}

NodeResult CBossKnightContinueAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossKnightContinueAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossKnightContinueAttackNode::Invoke(float DeltaTime)
{
	return NodeResult();
}
