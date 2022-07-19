#include "BossKnightWalkNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../KnightDataComponent.h"

CBossKnightWalkNode::CBossKnightWalkNode()
{
	SetTypeID(typeid(CBossKnightWalkNode).hash_code());
}

CBossKnightWalkNode::CBossKnightWalkNode(const CBossKnightWalkNode& Node)	:
	CActionNode(Node)
{
}

CBossKnightWalkNode::~CBossKnightWalkNode()
{
}

void CBossKnightWalkNode::Init()
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 애니메이션 콜백 등록
	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	AnimInst->AddNotifyDeltaTimeFrameRange("Walk", "Walk", 0, 5, Data, &CKnightDataComponent::OnWalk);
	AnimInst->AddNotifyDeltaTimeFrameRange("Walk", "Walk", 14, 24, Data, &CKnightDataComponent::OnWalk);
	AnimInst->AddNotifyDeltaTimeFrameRange("Walk", "Walk", 32, 41, Data, &CKnightDataComponent::OnWalk);
}

NodeResult CBossKnightWalkNode::OnStart(float DeltaTime)
{
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation("Walk");

	return NodeResult::Node_True;
}

NodeResult CBossKnightWalkNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossKnightWalkNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}
