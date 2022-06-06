
#include "ConditionNode.h"

CConditionNode::CConditionNode()
{
	m_NodeType = Node_Type::Condition;
}

CConditionNode::CConditionNode(const CConditionNode& Node)	:
	CNode(Node)
{
}

CConditionNode::~CConditionNode()
{
}

bool CConditionNode::Invoke(float DeltaTime)
{
	NodeResult Result =	OnStart(DeltaTime);

	if (Result == NodeResult::Node_False)
		return false;

	else if (Result == NodeResult::Node_True)
		return true;

	return true;
}
