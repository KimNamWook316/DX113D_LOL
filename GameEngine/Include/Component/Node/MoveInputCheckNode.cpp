
#include "MoveInputCheckNode.h"
#include "../../Input.h"

CMoveInputCheckNode::CMoveInputCheckNode()
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
	const keyState State = CInput::GetInst()->FindKeyState('W');

	if (!State.State[KeyState_Up] && !State.State[KeyState_Down] && !State.State[KeyState_Push])
	{
		return NodeResult::Node_False;
	}

	else
	{
		return NodeResult::Node_True;
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
