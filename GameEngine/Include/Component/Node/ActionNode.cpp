
#include "ActionNode.h"
#include "../BehaviorTree.h"

CActionNode::CActionNode()	:
	m_AnimationMeshComp(nullptr)
{
	SetTypeID(typeid(CActionNode).hash_code());
	m_NodeType = Node_Type::Action;
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

	NodeResult Result = OnUpdate(DeltaTime);

	if (m_IsEnd)
		OnEnd(DeltaTime);

	if (Result == NodeResult::Node_True)
		return true;
	else
		return false;
}

