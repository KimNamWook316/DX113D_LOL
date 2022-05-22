
#include "ActionNode.h"
#include "../BehaviorTree.h"

CActionNode::CActionNode()	:
	m_AnimationMeshComp(nullptr)
{
	SetTypeID(typeid(CActionNode).hash_code());
}

CActionNode::CActionNode(const CActionNode& Node)	:
	CNode(Node)
{
}

CActionNode::~CActionNode()
{
}

void CActionNode::SetAnimationMeshComponent(CAnimationMeshComponent* AnimationMeshComp)
{
	m_AnimationMeshComp = AnimationMeshComp;
}

CAnimationMeshComponent* CActionNode::GetAnimationMeshComponent() const
{
	return m_AnimationMeshComp;
}

bool CActionNode::Invoke(float DeltaTime)
{
	if(!m_CallStart)
		OnStart(DeltaTime);	

	OnUpdate(DeltaTime);

	if (m_IsEnd)
		OnEnd(DeltaTime);

	return true;
}

