
#include "MouseLButtonCheckNode.h"
#include "Input.h"

CMouseLButtonCheckNode::CMouseLButtonCheckNode()	:
	m_FrameCount(0)
{
	SetTypeID(typeid(CMouseLButtonCheckNode).hash_code());
}

CMouseLButtonCheckNode::CMouseLButtonCheckNode(const CMouseLButtonCheckNode& Node)	:
	CConditionNode(Node)
{
}

CMouseLButtonCheckNode::~CMouseLButtonCheckNode()
{
}

NodeResult CMouseLButtonCheckNode::OnStart(float DeltaTime)
{
	++m_FrameCount;
	bool Click = CInput::GetInst()->GetMouseLButtonClick();

	if (m_FrameCount > 20 && Click)
	{
		m_FrameCount = 0;
		return NodeResult::Node_True;
	}

	else
		return NodeResult::Node_False;
}

NodeResult CMouseLButtonCheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CMouseLButtonCheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CMouseLButtonCheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}