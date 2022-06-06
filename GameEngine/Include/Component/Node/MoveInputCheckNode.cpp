
#include "MoveInputCheckNode.h"
#include "../../Input.h"
#include "../BehaviorTree.h"

CMoveInputCheckNode::CMoveInputCheckNode()	:
	m_FrameCount(0)
{
	SetTypeID(typeid(CMoveInputCheckNode).hash_code());
}

CMoveInputCheckNode::CMoveInputCheckNode(const CMoveInputCheckNode& Node)	:
	CConditionNode(Node)
{
}

CMoveInputCheckNode::~CMoveInputCheckNode()
{
}

NodeResult CMoveInputCheckNode::OnStart(float DeltaTime)
{
	++m_FrameCount;

	if ((CInput::GetInst()->GetMouseRButtonClick() && m_FrameCount > 20))
	{
		m_FrameCount = 0;
		m_Object->ClearPath();
		//m_Owner->SetCurrentNode(nullptr);
		return NodeResult::Node_True;
	}

	else
	{
		return NodeResult::Node_False;
	}

}

NodeResult CMoveInputCheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CMoveInputCheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CMoveInputCheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
