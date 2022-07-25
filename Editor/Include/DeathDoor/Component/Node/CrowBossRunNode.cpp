
#include "CrowBossRunNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../CrowBossDataComponent.h"
#include "../MonsterNavAgent.h"

CCrowBossRunNode::CCrowBossRunNode()
{
	SetTypeID(typeid(CCrowBossRunNode).hash_code());
}

CCrowBossRunNode::CCrowBossRunNode(const CCrowBossRunNode& Node)	:
	CActionNode(Node)
{
}

CCrowBossRunNode::~CCrowBossRunNode()
{
}

void CCrowBossRunNode::Init()
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();
}

NodeResult CCrowBossRunNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("Run");

	return NodeResult::Node_True;
}

NodeResult CCrowBossRunNode::OnUpdate(float DeltaTime)
{
	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	CMonsterNavAgent* Agent = Data->GetMonsterNavAgent();

	Vector3 Dir = Agent->GetCurrentFaceDir();
	
	float Speed = Data->GetMoveSpeed();

	m_Object->AddWorldPos(Dir * Speed);

	return NodeResult::Node_True;
}

NodeResult CCrowBossRunNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
