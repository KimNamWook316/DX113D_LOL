
#include "Node.h"
#include "../BehaviorTree.h"

CNode::CNode()	:
	m_IsEnd(false),
	m_CallStart(false)
{
	SetTypeID(typeid(CNode).hash_code());
}

CNode::CNode(const CNode& Node)
{
}

CNode::~CNode()
{
}

void CNode::SetOwner(CBehaviorTree* Owner)
{
	m_Owner = Owner;
}

CBehaviorTree* CNode::GetOwner()
{
	return m_Owner;
}
