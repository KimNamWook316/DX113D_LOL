#include "DodgerFirstAttackNode.h"

CDodgerFirstAttackNode::CDodgerFirstAttackNode()
{
	SetTypeID(typeid(CDodgerFirstAttackNode).hash_code());
}

CDodgerFirstAttackNode::CDodgerFirstAttackNode(const CDodgerFirstAttackNode& Node)	:
	CActionNode(Node)
{	
}

CDodgerFirstAttackNode::~CDodgerFirstAttackNode()
{
}

void CDodgerFirstAttackNode::Init()
{
}

NodeResult CDodgerFirstAttackNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CDodgerFirstAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CDodgerFirstAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
