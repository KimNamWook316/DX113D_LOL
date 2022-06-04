
#include "MoveInputCheckNode.h"
#include "../../Input.h"


CMoveInputCheckNode::CMoveInputCheckNode()	:
	m_FrameCount(0)
{
}

CMoveInputCheckNode::CMoveInputCheckNode(const CMoveInputCheckNode& Node)
{
}

CMoveInputCheckNode::~CMoveInputCheckNode()
{
}

NodeResult CMoveInputCheckNode::OnStart(float DeltaTime)
{
	++m_FrameCount;

	if (CInput::GetInst()->GetMouseRButtonClick() && m_FrameCount > 20)
	{
		m_FrameCount = 0;
		return NodeResult::Node_True;
	}

	else
	{
		return NodeResult::Node_False;
	}

}

NodeResult CMoveInputCheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CMoveInputCheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CMoveInputCheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
