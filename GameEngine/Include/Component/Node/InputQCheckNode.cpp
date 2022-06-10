
#include "InputQCheckNode.h"
#include "../../Input.h"

CInputQCheckNode::CInputQCheckNode()
{
	SetTypeID(typeid(CInputQCheckNode).hash_code());
}

CInputQCheckNode::CInputQCheckNode(const CInputQCheckNode& Node)
{
}

CInputQCheckNode::~CInputQCheckNode()
{
}

NodeResult CInputQCheckNode::OnStart(float DeltaTime)
{
	const keyState State = CInput::GetInst()->FindKeyState('Q');

	if (!State.State[KeyState_Up] && !State.State[KeyState_Down] && !State.State[KeyState_Push])
	{
		return NodeResult::Node_False;
	}

	else
	{
		return NodeResult::Node_True;
	}
}

NodeResult CInputQCheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CInputQCheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CInputQCheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
