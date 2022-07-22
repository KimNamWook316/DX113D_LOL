#include "BossBettyChangeAttackDirNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../BossBettyDataComponent.h"

CBossBettyChangeAttackDirNode::CBossBettyChangeAttackDirNode()
{
	SetTypeID(typeid(CBossBettyChangeAttackDirNode).hash_code());
}

CBossBettyChangeAttackDirNode::CBossBettyChangeAttackDirNode(const CBossBettyChangeAttackDirNode& Node)
{
}

CBossBettyChangeAttackDirNode::~CBossBettyChangeAttackDirNode()
{
}

void CBossBettyChangeAttackDirNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 공중으로 뛰어 오르면서, Player 가 있는 쪽으로 공격 방향을 바꾼다.
	std::string AnimName = "BackUpStep";

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	
	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 19,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);
}

NodeResult CBossBettyChangeAttackDirNode::OnStart(float DeltaTime)
{

	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("BackUpStep");

	// Player 쪽으로 계속 돌기
	int CurrentFrame = m_AnimationMeshComp->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetCurrentFrameIdx();

	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	
	if (CurrentFrame >= 0 && CurrentFrame <= 19)
	{
		// CMonsterDataComponent::OnLookPlayer(DeltaTime);
		Data->OnLookPlayer(DeltaTime);
	}

	return NodeResult::Node_True;
}

NodeResult CBossBettyChangeAttackDirNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettyChangeAttackDirNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
