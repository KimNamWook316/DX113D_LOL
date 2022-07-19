#include "BossBettyJumpAttackNode.h"

CBossBettyJumpAttackNode::CBossBettyJumpAttackNode()
{
}

CBossBettyJumpAttackNode::CBossBettyJumpAttackNode(const CBossBettyJumpAttackNode& Node)
{
}

CBossBettyJumpAttackNode::~CBossBettyJumpAttackNode()
{
}

void CBossBettyJumpAttackNode::Init()
{
}

NodeResult CBossBettyJumpAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	return NodeResult();
}

NodeResult CBossBettyJumpAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettyJumpAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
