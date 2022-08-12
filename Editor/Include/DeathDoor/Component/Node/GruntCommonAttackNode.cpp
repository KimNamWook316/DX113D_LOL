#include "GruntCommonAttackNode.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "../HeadRollerDataComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../GruntCommonDataComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CGruntCommonAttackNode::CGruntCommonAttackNode()
{
	SetTypeID(typeid(CGruntCommonAttackNode).hash_code());
}

CGruntCommonAttackNode::CGruntCommonAttackNode(const CGruntCommonAttackNode& Node)
{
}

CGruntCommonAttackNode::~CGruntCommonAttackNode()
{
}

void CGruntCommonAttackNode::Init()
{
	CGruntCommonDataComponent* Data = dynamic_cast<CGruntCommonDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 1) Attack Start
	std::string AnimName = "AttackStart";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	AnimInst->AddNotify(AnimName, "EnableLook", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "DisableZMove", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);
	AnimInst->AddNotify(AnimName, "ChangeToAttack", 20,
		this, &CGruntCommonAttackNode::ChangeToAttackMainAnim);

	// 2. Attack Main
	AnimName = "AttackMain";
	AnimInst->AddNotify(AnimName, "SpeedInc", 0,
		this, &CGruntCommonAttackNode::IncMoveSpeed);
	AnimInst->AddNotify(AnimName, "DisableLookPlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
	AnimInst->AddNotify(AnimName, "EnableZMove", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableMoveZ);
	AnimInst->AddNotify(AnimName, "EnableAttackCollider", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnActiveMeleeAttackCollider);

	AnimInst->AddNotify(AnimName, "SpeedToOrigin", 8,
		this, &CGruntCommonAttackNode::IncMoveSpeed);
	AnimInst->AddNotify(AnimName, "DisableZMove", 8,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);
	AnimInst->AddNotify(AnimName, "DisabeAttackCollider", 8,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnInActiveMeleeAttackCollider);

	AnimInst->AddNotify(AnimName, "SetCurrentNodeNull", 34,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::SetCurrentNodeNull);
}

NodeResult CGruntCommonAttackNode::OnStart(float DeltaTime)
{
	CGruntCommonDataComponent* Data = dynamic_cast<CGruntCommonDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	if (m_AnimationMeshComp->GetAnimationInstance()->GetCurrentAnimation()->GetName() != "AttackMain")
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("AttackStart");
	}

	m_Owner->SetCurrentNode(this);

	return NodeResult::Node_True;
}

NodeResult CGruntCommonAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CGruntCommonAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

void CGruntCommonAttackNode::IncMoveSpeed()
{
	CGruntCommonDataComponent* Data = dynamic_cast<CGruntCommonDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	Data->SetCurMoveSpeed(Data->GetGruntOriginSpeed() * 2.f);
}

void CGruntCommonAttackNode::ChangeToAttackMainAnim()
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("AttackMain");
}
