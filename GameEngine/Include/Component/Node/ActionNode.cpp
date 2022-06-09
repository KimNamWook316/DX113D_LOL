
#include "ActionNode.h"
#include "../BehaviorTree.h"

CActionNode::CActionNode()
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

