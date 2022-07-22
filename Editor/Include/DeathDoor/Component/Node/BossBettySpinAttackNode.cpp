#include "BossBettySpinAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../BossBettyDataComponent.h"

CBossBettySpinAttackNode::CBossBettySpinAttackNode()
{
	SetTypeID(typeid(CBossBettySpinAttackNode).hash_code());
}

CBossBettySpinAttackNode::CBossBettySpinAttackNode(const CBossBettySpinAttackNode& Node)
{
}

CBossBettySpinAttackNode::~CBossBettySpinAttackNode()
{
}

void CBossBettySpinAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 1) JumpSpin
	std::string AnimName = "JumpSpin";

	// JumpSpin Animation 이 끝나면 바로 Spin Animation 으로 바꾼다.
	AnimInst->SetEndFunction(AnimName,
		this, &CBossBettySpinAttackNode::OnBossBettyChangeToSpinAnimation);

	// 2) Spin
	AnimName = "Spin";

	// Spin 을 하는 순간, 처음에 ColliderBox 에 특정 Callback 을 건다.
	// - 벽 등과 충돌 시에 SpinCollide 로 바꿔야 한다.
	// - 해당 ColliderBox 및, 콜백함수는 이미 BossBettyDataComponent 에서 세팅된 상태이다.
	// - 이를 다시 Enable 만 시켜주면 된다.
	AnimInst->AddNotify(AnimName, "EnableSpinCollider", 0,
		Data, &CBossBettyDataComponent::OnBossBettyEnableSpinCollider);

	// 2) Spin Collide
	AnimName = "SpinCollide";

	// 처음 시작하는 순간, 다시 Spin Collide 를 비활성화 시킨다.
	AnimInst->AddNotify(AnimName, "DisableSpinCollider", 0, 
		Data, &CBossBettyDataComponent::OnBossBettyDisableSpinCollider);

	// 공중에서 Player 를 향해 서서히 돈다.
	// 던지기 전까지 Player 방향으로 회전할 수 있도록 한다.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 16,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);

	// 마지막 순간에 착지한 바닥을 공격한다.
	AnimInst->SetEndFunction(AnimName,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// 공중에서 회전하는 중간에 Player 쪽으로 서서히 방향을 튼다.
}

NodeResult CBossBettySpinAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation("JumpSpin");

	return NodeResult::Node_True;
}

NodeResult CBossBettySpinAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettySpinAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

void CBossBettySpinAttackNode::OnBossBettyChangeToSpinAnimation()
{
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation("Spin");
}

