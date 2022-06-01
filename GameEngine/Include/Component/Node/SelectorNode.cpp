
#include "SelectorNode.h"
#include "../BehaviorTree.h"

CSelectorNode::CSelectorNode()
{
	SetTypeID(typeid(CSelectorNode).hash_code());
}

CSelectorNode::CSelectorNode(const CSelectorNode& Node)	:
	CCompositeNode(Node)
{
}

CSelectorNode::~CSelectorNode()
{
}

bool CSelectorNode::Invoke(float DeltaTime)
{
	auto iter = GetChildrenList().begin();
	auto iterEnd = GetChildrenList().end();

	for (; iter != iterEnd; ++iter)
	{
		// Selector는 하나의 자식이라도 true를 리턴하면 바로 자신도 true를 리턴
		if ((*iter)->Invoke(DeltaTime))
			return true;
	}

	SetChildrenCallStart(false);
	return false;
}

NodeResult CSelectorNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSelectorNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSelectorNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
