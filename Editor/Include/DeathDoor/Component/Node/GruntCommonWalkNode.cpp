#include "GruntCommonWalkNode.h"

CGruntCommonWalkNode::CGruntCommonWalkNode()
{
	SetTypeID(typeid(CGruntCommonWalkNode).hash_code());
}

CGruntCommonWalkNode::CGruntCommonWalkNode(const CGruntCommonWalkNode& Node)
{
}

CGruntCommonWalkNode::~CGruntCommonWalkNode()
{
}

void CGruntCommonWalkNode::Init()
{
}

NodeResult CGruntCommonWalkNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CGruntCommonWalkNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CGruntCommonWalkNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
