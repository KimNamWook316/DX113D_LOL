#include "DodgerPostAttackNode.h"

CDodgerPostAttackNode::CDodgerPostAttackNode()
{
	SetTypeID(typeid(CDodgerPostAttackNode).hash_code());
}

CDodgerPostAttackNode::CDodgerPostAttackNode(const CDodgerPostAttackNode& Node)	:
	CActionNode(Node)
{
}

CDodgerPostAttackNode::~CDodgerPostAttackNode()
{
}

void CDodgerPostAttackNode::Init()
{
}

NodeResult CDodgerPostAttackNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CDodgerPostAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CDodgerPostAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
