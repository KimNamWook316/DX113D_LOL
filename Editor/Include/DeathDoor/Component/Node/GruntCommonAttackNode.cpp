#include "GruntCommonAttackNode.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "../MonsterNavAgent.h"
#include "../HeadRollerDataComponent.h"
#include "Component/AnimationMeshComponent.h"

CGruntCommonAttackNode::CGruntCommonAttackNode()
{
	SetTypeID(typeid(CGruntCommonAttackNode).hash_code());
}

CGruntCommonAttackNode::CGruntCommonAttackNode(const CGruntCommonAttackNode& Node)
{
}

CGruntCommonAttackNode::~CGruntCommonAttackNode()
{
}

void CGruntCommonAttackNode::Init()
{
}

NodeResult CGruntCommonAttackNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CGruntCommonAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CGruntCommonAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
